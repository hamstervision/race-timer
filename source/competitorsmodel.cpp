#include "competitorsmodel.h"

const QString CompetitorsModel::CompetitorsTag("competitors");

CompetitorsModel::CompetitorsModel(CategoriesModel *categoriesModel, QObject *parent)
    : QAbstractTableModel(parent)
    , m_categoriesModel(categoriesModel)
    , m_dirty(false)
{
    Q_ASSERT(m_categoriesModel);
}

CompetitorsModel::~CompetitorsModel()
{
    clear();
}

void CompetitorsModel::addCompetitor(Competitor *competitor)
{
    if (!competitor)
    {
        Q_ASSERT(false);
        return;
    }

    int row = (int)m_competitors.size();
    beginInsertRows(QModelIndex(), row, row);

    competitor->setParent(this);

    m_competitors[competitor->raceNumber()] = competitor;

    endInsertRows();

    m_dirty = true;
}

size_t CompetitorsModel::competitorCount() const
{
    return m_competitors.size();
}

bool CompetitorsModel::serialise(QDomDocument &xmlDoc, QDomElement &rootNode)
{
    QDomElement el = xmlDoc.createElement(CompetitorsTag);

    for (auto it : m_competitors)
    {
        it.second->serialise(xmlDoc, el);
    }

    rootNode.appendChild(el);

    m_dirty = false;

    return true;
}

bool CompetitorsModel::deserialise(QDomElement &rootNode)
{
    clear();

    QDomNodeList competitorsNodes(rootNode.elementsByTagName(CompetitorsTag));
    if (competitorsNodes.size() != 1)
    {
        Q_ASSERT(false);
        return false;
    }

    QDomElement competitorsEl(competitorsNodes.at(0).toElement());
    QDomElement competitorEl(competitorsEl.firstChildElement());

    int competitorCount = competitorsEl.elementsByTagName(Competitor::CompetitorTag).size();

    int row = (int)m_competitors.size();
    beginInsertRows(QModelIndex(), row, row + competitorCount - 1);

    while (!competitorEl.isNull())
    {
        if (competitorEl.tagName().compare(Competitor::CompetitorTag, Qt::CaseInsensitive) != 0
                || !competitorEl.hasAttribute(Competitor::RaceNumberAttr)
                || !competitorEl.hasAttribute(Competitor::FirstNameAttr)
                || !competitorEl.hasAttribute(Competitor::LastNameAttr)
                || !competitorEl.hasAttribute(Competitor::CategoryAttr))
        {
            Q_ASSERT(false);
            competitorEl = competitorEl.nextSiblingElement();
            continue;
        }

        Category *category = m_categoriesModel->category(competitorEl.attribute(Competitor::CategoryAttr).toUInt());
        if (!category)
        {
            Q_ASSERT(false);
            competitorEl = competitorEl.nextSiblingElement();
            continue;
        }

        Competitor *competitor = new Competitor(this);

        uint raceNumber = competitorEl.attribute(Competitor::RaceNumberAttr).toUInt();

        competitor->setRaceNumber(raceNumber);
        competitor->setFirstName(competitorEl.attribute(Competitor::FirstNameAttr));
        competitor->setLastName(competitorEl.attribute(Competitor::LastNameAttr));
        competitor->setCategory(category);

        // Membership ID is optional
        if (competitorEl.hasAttribute(Competitor::MembershipIdAttr))
            competitor->setMembershipId(competitorEl.attribute(Competitor::MembershipIdAttr));

        m_competitors[raceNumber] = competitor;

        competitorEl = competitorEl.nextSiblingElement();
    }

    endInsertRows();

    m_dirty = false;

    return true;
}

Competitor* CompetitorsModel::competitor(uint raceNumber)
{
    auto it = m_competitors.find(raceNumber);
    if (it == m_competitors.end())
        return nullptr;

    return it->second;
}

bool CompetitorsModel::dirty() const
{
    return m_dirty;
}

QVariant CompetitorsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case RaceNumberColumn:
            return QString("Race Number");
        case FirstNameColumn:
            return QString("First Name");
        case LastNameColumn:
            return QString("Last Name");
        case CategoryColumn:
            return QString("Category");
        case MembershipIdColumn:
            return QString("Membership ID (Optional)");
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return QVariant();
}

int CompetitorsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return (int)m_competitors.size();

    return 0;
}

int CompetitorsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant CompetitorsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole || !index.isValid())
        return QVariant();

    int row = index.row();
    uint raceNumber = row + 1;

    auto it = m_competitors.find(raceNumber);
    if (it == m_competitors.end())
    {
        Q_ASSERT(false);
        return QVariant();
    }

    Competitor *competitor = it->second;

    switch (index.column())
    {
    case RaceNumberColumn:
        return competitor->raceNumber();
    case FirstNameColumn:
        return competitor->firstName();
    case LastNameColumn:
        return competitor->lastName();
    case CategoryColumn:
        {
            Category *category = competitor->category();
            if (category)
                return category->name();
            else
                return QString();
        }
    case MembershipIdColumn:
        return competitor->membershipId();
    default:
        break;
    }

    return QVariant();
}

bool CompetitorsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (data(index, role) != value)
        {
            int row = index.row();
            uint raceNumber = row + 1;

            auto it = m_competitors.find(raceNumber);
            if (it == m_competitors.end())
            {
                Q_ASSERT(false);
                return false;
            }

            Competitor *competitor = it->second;

            switch(index.column())
            {
            case RaceNumberColumn:
                competitor->setRaceNumber(value.toUInt());
                break;
            case FirstNameColumn:
                competitor->setFirstName(value.toString());
                break;
            case LastNameColumn:
                competitor->setLastName(value.toString());
                break;
            case CategoryColumn:
                {
                    QString newCategory(value.toString());
                    const std::vector<Category*> &categories = m_categoriesModel->categories();
                    for (auto category : categories)
                    {
                        if (newCategory.compare(category->name()) == 0)
                        {
                            competitor->setCategory(category);
                            break;
                        }
                    }
                }
                break;
            case MembershipIdColumn:
                competitor->setMembershipId(value.toString());
                break;
            default:
                Q_ASSERT(false);
                return false;
            }

            m_dirty = true;

            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
    }
    return true;
}

bool CompetitorsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    int clearedCount = 0;
    for (int i = 0; i < count; i++)
    {
        auto it = m_competitors.find(row + 1 + i);
        if (it == m_competitors.end())
        {
            Q_ASSERT(false);
            continue;
        }

        it->second->clear();

        ++clearedCount;
    }

    emit dataChanged(index(row, FirstNameColumn), index(row + count, MembershipIdColumn));

    return (clearedCount > 0);
}

Qt::ItemFlags CompetitorsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == RaceNumberColumn)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void CompetitorsModel::onCategoriesChanged()
{
    emit dataChanged(index(0, CategoryColumn), index((int)m_competitors.size() - 1, CategoryColumn));
}

void CompetitorsModel::clear()
{
    if (m_competitors.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, (int)m_competitors.size() - 1);

    for (auto it : m_competitors)
    {
        delete it.second;
    }
    m_competitors.clear();

    endRemoveRows();
}
