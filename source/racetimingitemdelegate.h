#ifndef RACETIMINGITEMDELEGATE_H
#define RACETIMINGITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "racetimingmodel.h"

class RaceTimingItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RaceTimingItemDelegate(RaceTimingModel *model, QObject *parent = nullptr);

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
    RaceTimingModel *m_raceTimingModel;
};

#endif // RACETIMINGITEMDELEGATE_H
