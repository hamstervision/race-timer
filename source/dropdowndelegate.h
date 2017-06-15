#ifndef DROPDOWNDELEGATE_H
#define DROPDOWNDELEGATE_H

#include <QStyledItemDelegate>
#include <QStringList>

class DropDownDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DropDownDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setDropDownStrings(const QStringList &strings);
protected:
    QStringList m_dropDownStrings;
};

#endif // DROPDOWNDELEGATE_H
