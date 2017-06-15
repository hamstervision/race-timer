#include "racetimingmodel.h"

#include <QDomElement>

static const QString TimeColumnName("Finish Time");
static const QString RaceNumberColumnName("Race Number");
static const QString NameColumnName("Name");

static const QString ResultsXmlNode("results");

RaceTimingModel::RaceTimingModel(CompetitorsModel *competitorsModel, QObject *parent)
    : QAbstractTableModel(parent)
    , m_competitorsModel(competitorsModel)
    , m_errorBrush(QColor(0xde, 0x68, 0x68))
    , m_dirty(false)
{
    Q_ASSERT(competitorsModel);
}

RaceTimingModel::~RaceTimingModel()
{
    clear();
}

void RaceTimingModel::addResult(const uint raceNumber, const QDateTime &finishTime)
{
    int row = (int)m_results.size();
    beginInsertRows(QModelIndex(), row, row);
    m_results.push_back(new Result(raceNumber, finishTime));
    updateRaceNumberCounts();
    endInsertRows();
    emit resultsChanged();
}

Results RaceTimingModel::results(Category *category) const
{
    if (!category)
        return m_results;

    Results results;
    for (auto && result : m_results)
    {
        Competitor *competitor = m_competitorsModel->competitor(result->raceNumber());
        if (!competitor)
            continue;

        if (competitor->category() == category)
            results.push_back(result);
    }

    return results;
}

QVariant RaceTimingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case TimeColumnId:
            return TimeColumnName;
        case RaceNumberColumnId:
            return RaceNumberColumnName;
        case NameColumnId:
            return NameColumnName;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return QVariant();
}


int RaceTimingModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)m_results.size();
}

int RaceTimingModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant RaceTimingModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    if (row >= m_results.size())
    {
        Q_ASSERT(false);
        return QVariant();
    }

    Result *result = m_results.at(row);
    if (!result)
    {
        Q_ASSERT(false);
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case TimeColumnId:
            return result->finishTime();
        case RaceNumberColumnId:
            return result->raceNumber();
        case NameColumnId:
            {
                Competitor *competitor = m_competitorsModel->competitor(result->raceNumber());
                return competitor ? QString("%1 %2").arg(competitor->firstName(), competitor->lastName()) : QString();
            }
        default:
            break;
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        auto itNumber = m_raceNumberCounts.find(result->raceNumber());
        if (itNumber != m_raceNumberCounts.end() && itNumber->second > 1)
            return m_errorBrush;

        if (index.column() == NameColumnId)
        {
            Competitor *competitor = m_competitorsModel->competitor(result->raceNumber());
            if (!competitor)
                return m_errorBrush;
        }
    }
    return QVariant();
}

bool RaceTimingModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    beginRemoveRows(QModelIndex(), row, row + count);

    int removedCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (row >= (int)m_results.size())
        {
            Q_ASSERT(false);
            continue;
        }

        Result *result = m_results.at(row);
        if (!result)
        {
            Q_ASSERT(false);
            continue;
        }

        m_results.erase(m_results.begin() + row);
        delete result;
        ++removedCount;
    }

    updateRaceNumberCounts();

    endRemoveRows();

    emit resultsChanged();

    return (removedCount > 0);
}

bool RaceTimingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
    {
        if (data(index, role) != value)
        {
            Result *result = m_results.at(index.row());
            if (!result)
            {
                Q_ASSERT(false);
                return false;
            }

            switch(index.column())
            {
            case RaceNumberColumnId:
                result->setRaceNumner(value.toUInt());
                updateRaceNumberCounts();
                break;
            case TimeColumnId:
                result->setFinishTime(value.toDateTime());
                break;
            default:
                return false;
            }

            emit dataChanged(index, index, QVector<int>() << role);
            emit resultsChanged();
            m_dirty = true;
            return true;
        }
    }
    return false;
}

Qt::ItemFlags RaceTimingModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    if (index.column() == RaceNumberColumnId || index.column() == TimeColumnId)
        flags |= Qt::ItemIsEditable;

    return flags;
}

bool RaceTimingModel::serialise(QDomDocument &xmlDoc, QDomElement &rootNode)
{
    QDomElement el = xmlDoc.createElement(ResultsXmlNode);

    for (auto result : m_results)
    {
        result->serialise(xmlDoc, el);
    }

    rootNode.appendChild(el);

    m_dirty = false;

    return true;
}

bool RaceTimingModel::deserialise(QDomElement &rootNode)
{
    clear();

    QDomNodeList resultsNodes(rootNode.elementsByTagName(ResultsXmlNode));
    if (resultsNodes.size() != 1)
    {
        Q_ASSERT(false);
        return false;
    }

    QDomElement resultsEl(resultsNodes.at(0).toElement());

    int resultsCount = resultsEl.elementsByTagName(Result::ResultXmlTag).size();

    QDomElement resultEl(resultsEl.firstChildElement());

    if (resultsCount <= 0)
    {
        m_dirty = false;
        emit resultsChanged();
        return true;
    }

    while (!resultEl.isNull())
    {
        if (resultEl.tagName().compare(Result::ResultXmlTag, Qt::CaseInsensitive) != 0
                || !resultEl.hasAttribute(Result::RaceNumberXmlAttrib)
                || !resultEl.hasAttribute(Result::FinishTimeXmlAttrib))
        {
            Q_ASSERT(false);
            resultEl = resultEl.nextSiblingElement();
            continue;
        }

        uint raceNumber = resultEl.attribute(Result::RaceNumberXmlAttrib).toUInt();
        QDateTime finishTime = QDateTime::fromString(resultEl.attribute(Result::FinishTimeXmlAttrib), Qt::ISODate);

        addResult(raceNumber, finishTime);

        resultEl = resultEl.nextSiblingElement();
    }

    m_dirty = false;

    emit resultsChanged();

    return true;
}

bool RaceTimingModel::dirty() const
{
    return m_dirty;
}

void RaceTimingModel::clear()
{
    if (m_results.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, (int)m_results.size() - 1);
    for (auto result : m_results)
    {
        delete result;
    }
    m_results.clear();
    m_raceNumberCounts.clear();
    endRemoveRows();
}

void RaceTimingModel::updateRaceNumberCounts()
{
    m_raceNumberCounts.clear();
    for (auto result : m_results)
    {
        auto it = m_raceNumberCounts.find(result->raceNumber());
        if (it != m_raceNumberCounts.end())
        {
            ++it->second;
        }
        else
        {
            m_raceNumberCounts[result->raceNumber()] = 1;
        }
    }
}
