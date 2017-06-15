#ifndef RACETIMINGMODEL_H
#define RACETIMINGMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QBrush>
#include <vector>
#include <map>
#include "competitorsmodel.h"
#include "result.h"

class RaceTimingModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RaceTimingModel(CompetitorsModel *competitorsModel, QObject *parent = nullptr);
    ~RaceTimingModel();

    void addResult(const uint raceNumber, const QDateTime &finishTime);
    Results results(Category *category) const;

    static const int TimeColumnId       = 0;
    static const int RaceNumberColumnId = 1;
    static const int NameColumnId       = 2;
    static const int ColumnCount        = 3;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Remove rows:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool serialise(QDomDocument &xmlDoc, QDomElement &rootNode);
    bool deserialise(QDomElement &rootNode);

    bool dirty() const;
    void clear();

signals:
    void resultsChanged();

protected:
    void updateRaceNumberCounts();

protected:
    CompetitorsModel *m_competitorsModel;
    Results m_results;
    std::map<uint, uint> m_raceNumberCounts;
    QBrush m_errorBrush;
    bool m_dirty;
};

#endif // RACETIMINGMODEL_H
