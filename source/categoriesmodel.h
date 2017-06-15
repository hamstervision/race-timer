#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QAbstractTableModel>
#include <QDomDocument>
#include <QDomElement>
#include <map>

#include "category.h"

class CategoriesModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CategoriesModel(QObject *parent = nullptr);
    virtual ~CategoriesModel();

    static const int NameColumnId       = 0;
    static const int StartTimeColumnId  = 1;
    static const int ColumnCount        = 2;

    static const QString CategoriesTag;

    void addCategory();
    void removeCategory(const QModelIndex &toBeDeleted);

    std::vector<Category*> categories() const;

    Category* category(uint id) const;

    bool serialise(QDomDocument &xmlDoc, QDomElement &rootNode);
    bool deserialise(QDomElement &rootNode);

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

    Qt::ItemFlags flags(const QModelIndex& index) const override;

signals:
    void categoriesChanged();

protected:
    Category* categoryAt(const int row) const;

protected:
    std::map<uint, Category*> m_categories;
    uint m_nextCategoryId;
    bool m_dirty;
};

#endif // CATEGORIESMODEL_H
