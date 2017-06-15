#ifndef RACERESULTSMODEL_H
#define RACERESULTSMODEL_H

#include <QAbstractTableModel>

#include "competitorsmodel.h"
#include "racetimingmodel.h"

class RaceResultsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RaceResultsModel(CompetitorsModel *competitorsModel, RaceTimingModel *raceTimingModel, QObject *parent = nullptr);
    ~RaceResultsModel();

    static const int PositionColumnId   = 0;
    static const int TimeColumnId       = 1;
    static const int NameColumnId       = 2;
    static const int RaceNumberColumnId = 3;
    static const int ColumnCount        = 4;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    void setCategory(Category *category);

    void clear();

public slots:
    void onResultsChanged();

protected:
    void updateResults();
    qlonglong raceTimeFromResult(const Result *result) const;

protected:
    CompetitorsModel *m_competitorsModel;
    RaceTimingModel *m_raceTimingModel;
    Category *m_category;
    Results m_results;
};

#endif // RACERESULTSMODEL_H
