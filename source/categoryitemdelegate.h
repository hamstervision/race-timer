#ifndef CATEGORYITEMDELEGATE_H
#define CATEGORYITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "categoriesmodel.h"

class CategoryItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CategoryItemDelegate(CategoriesModel *categoriesModel, QObject *parent = nullptr);

public: // QStyledItemDelegate
    QString displayText(const QVariant &value, const QLocale &locale) const override;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    CategoriesModel *m_categoriesModel;
};

#endif // CATEGORYITEMDELEGATE_H
