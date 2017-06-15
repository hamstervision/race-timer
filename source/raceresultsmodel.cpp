#include "raceresultsmodel.h"

static const QString PositionColumnName("Position");
static const QString TimeColumnName("Time");
static const QString RaceNumberColumnName("Race Number");
static const QString NameColumnName("Name");

RaceResultsModel::RaceResultsModel(CompetitorsModel *competitorsModel, RaceTimingModel *raceTimingModel, QObject *parent /*= nullptr*/)
    : QAbstractTableModel(parent)
    , m_competitorsModel(competitorsModel)
    , m_raceTimingModel(raceTimingModel)
    , m_category(nullptr)
{
    Q_ASSERT(m_competitorsModel);
    Q_ASSERT(m_raceTimingModel);
}

RaceResultsModel::~RaceResultsModel()
{
    clear();
}

QVariant RaceResultsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case PositionColumnId:
            return PositionColumnName;
        case TimeColumnId:
            return TimeColumnName;
        case NameColumnId:
            return NameColumnName;
        case RaceNumberColumnId:
            return RaceNumberColumnName;
        default:
            Q_ASSERT(false);
            break;
        }
    }

    return QVariant();
}

int RaceResultsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return (int)m_results.size();
}

int RaceResultsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return ColumnCount;
}

QVariant RaceResultsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
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

        switch (index.column())
        {
        case PositionColumnId:
            return QVariant(row + 1);
        case TimeColumnId:
            {
                return QVariant(raceTimeFromResult(result));
            }
        case NameColumnId:
            {
                Competitor *competitor = m_competitorsModel->competitor(result->raceNumber());
                return competitor ? QString("%1 %2").arg(competitor->firstName(), competitor->lastName()) : QString();
            }
        case RaceNumberColumnId:
            return result->raceNumber();
        default:
            break;
        }
    }
    return QVariant();
}

Qt::ItemFlags RaceResultsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void RaceResultsModel::setCategory(Category *category)
{
    if (category == m_category)
        return;

    m_category = category;
    updateResults();
}

void RaceResultsModel::onResultsChanged()
{
    updateResults();
}

void RaceResultsModel::clear()
{
    if (m_results.empty())
        return;

    beginRemoveRows(QModelIndex(), 0, (int)m_results.size() - 1);
    m_results.clear();
    endRemoveRows();
}

void RaceResultsModel::updateResults()
{
    clear();
    Results results(m_raceTimingModel->results(m_category));

    if (results.empty())
        return;

    beginInsertRows(QModelIndex(), 0, (int)results.size() - 1);
    m_results = results;
    std::sort(m_results.begin(), m_results.end(), [this](const Result *lhs, const Result *rhs)
    {
       return raceTimeFromResult(lhs) < raceTimeFromResult(rhs);
    });
    endInsertRows();
}

qlonglong RaceResultsModel::raceTimeFromResult(const Result *result) const
{
    Competitor *competitor = result ? m_competitorsModel->competitor(result->raceNumber()) : nullptr;
    Category *category = competitor ? competitor->category() : nullptr;
    if (category)
        return category->startTime().msecsTo(result->finishTime());

    return -1ll;
}
