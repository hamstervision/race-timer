#include "categoriesmodel.h"

#include <QDateTime>
#include <QDomElement>
#include <QBrush>

static const QString NameColumnText("Name");
static const QString StartTimeColumnText("Start Time");

const QString CategoriesModel::CategoriesTag("categories");

CategoriesModel::CategoriesModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_nextCategoryId(0)
    , m_dirty(false)
{
}

CategoriesModel::~CategoriesModel()
{
    clear();
}

void CategoriesModel::addCategory()
{
    int row = (int)m_categories.size();
    beginInsertRows(QModelIndex(), row, row);

    Category *newCategory = new Category(this);
    newCategory->setId(m_nextCategoryId);
    newCategory->setStartTime(QDateTime::currentDateTimeUtc());

    m_categories[m_nextCategoryId] = newCategory;
    ++m_nextCategoryId;

    endInsertRows();

    emit categoriesChanged();

    m_dirty = true;
}

void CategoriesModel::removeCategory(const QModelIndex &toBeDeleted)
{
    if (!toBeDeleted.isValid())
        return;

    int row = toBeDeleted.row();

    beginRemoveRows(QModelIndex(), row, row);

    Category *category = categoryAt(row);

    if (!category)
    {
        Q_ASSERT(false);
        return;
    }

    m_categories.erase(category->id());
    delete category;

    endRemoveRows();

    emit categoriesChanged();

    m_dirty = true;
}

std::vector<Category *> CategoriesModel::categories() const
{
    std::vector<Category*> categories;

    for (auto it : m_categories)
    {
        categories.push_back(it.second);
    }

    return categories;
}

Category *CategoriesModel::category(uint id) const
{
    auto it = m_categories.find(id);
    if (it == m_categories.end())
        return nullptr;

    return it->second;
}

bool CategoriesModel::serialise(QDomDocument &xmlDoc, QDomElement &rootNode)
{
    QDomElement el = xmlDoc.createElement(CategoriesTag);

    for (auto it : m_categories)
    {
        it.second->serialise(xmlDoc, el);
    }

    rootNode.appendChild(el);

    m_dirty = false;

    return true;
}

bool CategoriesModel::deserialise(QDomElement &rootNode)
{
    clear();

    QDomNodeList categoriesNodes(rootNode.elementsByTagName(CategoriesTag));
    if (categoriesNodes.size() != 1)
    {
        Q_ASSERT(false);
        return false;
    }

    QDomElement categoriesEl(categoriesNodes.at(0).toElement());

    int categoriesCount = categoriesEl.elementsByTagName(Category::CategoryTag).size();

    QDomElement categoryEl(categoriesEl.firstChildElement());

    int row = (int)m_categories.size();
    beginInsertRows(QModelIndex(), row, row + categoriesCount - 1);

    uint maxId = 0;

    while (!categoryEl.isNull())
    {
        if (categoryEl.tagName().compare(Category::CategoryTag, Qt::CaseInsensitive) != 0
                || !categoryEl.hasAttribute(Category::IdAttr)
                || !categoryEl.hasAttribute(Category::NameAttr)
                || !categoryEl.hasAttribute(Category::StartTimeAttr))
        {
            Q_ASSERT(false);
            categoryEl = categoryEl.nextSiblingElement();
            continue;
        }

        Category *category = new Category(this);

        uint id = categoryEl.attribute(Category::IdAttr).toUInt();

        category->setId(id);
        category->setName(categoryEl.attribute(Category::NameAttr));
        category->setStartTime(QDateTime::fromString(categoryEl.attribute(Category::StartTimeAttr), Qt::ISODate));

        m_categories[id] = category;

        maxId = maxId < id ? id : maxId;

        categoryEl = categoryEl.nextSiblingElement();
    }

    endInsertRows();

    m_nextCategoryId = maxId + 1;

    m_dirty = false;

    emit categoriesChanged();

    return true;
}

bool CategoriesModel::dirty() const
{
    return m_dirty;
}


QVariant CategoriesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case NameColumnId:
            return NameColumnText;
        case StartTimeColumnId:
            return StartTimeColumnText;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return QVariant();
}

int CategoriesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)m_categories.size();
}

int CategoriesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        int row = index.row();

        Category *category = categoryAt(row);

        if (!category)
            return QVariant();

        switch (index.column())
        {
        case NameColumnId:
            return category->name();
        case StartTimeColumnId:
            return category->startTime();
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        if (index.column() == NameColumnId)
        {
            Category *category = categoryAt(index.row());

            for (auto it : m_categories)
            {
                if (it.second != category && category->name().compare(it.second->name(), Qt::CaseInsensitive) == 0)
                {
                    return QBrush(QColor(Qt::red));
                }
            }
        }
    }

    return QVariant();
}

bool CategoriesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (data(index, role) != value)
        {
            int row = index.row();
            Category *category = categoryAt(row);
            if (!category)
                return false;

            switch(index.column())
            {
            case NameColumnId:
                category->setName(value.toString());
                break;
            case StartTimeColumnId:
                category->setStartTime(value.toDateTime());
                break;
            default:
                Q_ASSERT(false);
                return false;
            }

            emit dataChanged(index, index, QVector<int>() << role);
            emit categoriesChanged();
            m_dirty = true;
            return true;
        }
    }
    return false;
}

Qt::ItemFlags CategoriesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

Category *CategoriesModel::categoryAt(const int row) const
{
    // Potentially no correlation between row and ID as categories may have been removed
    // Search required. :-/

    int i = 0;
    Category *category = nullptr;
    for (auto it : m_categories)
    {
        if (i == row)
        {
            category = it.second;
            break;
        }
        ++i;
    }

    return category;
}

void CategoriesModel::clear()
{
    m_nextCategoryId = 0;

    if (m_categories.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, (int)m_categories.size() - 1);

    for (auto it : m_categories)
    {
        delete it.second;
    }
    m_categories.clear();

    endRemoveRows();

    emit categoriesChanged();
}
