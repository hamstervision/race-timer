#ifndef COMPETITORITEMDELEGATE_H
#define COMPETITORITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "competitorsmodel.h"
#include "categoriesmodel.h"

class CompetitorItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CompetitorItemDelegate(CompetitorsModel *competitorsModel, CategoriesModel *categoriesModel, QObject *parent = nullptr);

public: // QStyledItemDelegate
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    CompetitorsModel *m_competitorsModel;
    CategoriesModel *m_categoriesModel;
};

#endif // COMPETITORITEMDELEGATE_H
