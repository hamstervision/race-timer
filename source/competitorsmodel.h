#ifndef COMPETITORSMODEL_H
#define COMPETITORSMODEL_H

#include <QAbstractTableModel>
#include <QDomDocument>
#include <QDomElement>
#include <map>

#include "competitor.h"
#include "categoriesmodel.h"

class CompetitorsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CompetitorsModel(CategoriesModel *categoriesModel, QObject *parent = nullptr);
    virtual ~CompetitorsModel();

    static const QString CompetitorsTag;

    static const int RaceNumberColumn   = 0;
    static const int FirstNameColumn    = 1;
    static const int LastNameColumn     = 2;
    static const int CategoryColumn     = 3;
    static const int MembershipIdColumn = 4;
    static const int ColumnCount        = 5;

    void addCompetitor(Competitor *competitor);

    size_t competitorCount() const;

    bool serialise(QDomDocument &xmlDoc, QDomElement &rootNode);
    bool deserialise(QDomElement &rootNode);

    Competitor* competitor(uint raceNumber);

    bool dirty() const;
    void clear();

public: // QAbstractTableModel

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    
    // Remove rows:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;    

    Qt::ItemFlags flags(const QModelIndex& index) const override;

protected slots:
    void onCategoriesChanged();

protected:
    CategoriesModel *m_categoriesModel;
    std::map<uint, Competitor*> m_competitors;
    bool m_dirty;
};

#endif // COMPETITORSMODEL_H
