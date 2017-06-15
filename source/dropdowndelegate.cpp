#include "dropdowndelegate.h"
#include <QComboBox>

DropDownDelegate::DropDownDelegate(QObject *parent /*= nullptr*/)
    : QStyledItemDelegate(parent)
{

}

QWidget *DropDownDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *combo = new QComboBox(parent);
    combo->setEditable(false);

    combo->addItems(m_dropDownStrings);

    return combo;
}

void DropDownDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *combo = dynamic_cast<QComboBox*>(editor);
    if (!combo)
    {
        Q_ASSERT(false);
        return;
    }

    combo->setEditText(value);
}

void DropDownDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *combo = dynamic_cast<QComboBox*>(editor);
    if (!combo)
    {
        Q_ASSERT(false);
        return;
    }

    QString text = combo->currentText();
    model->setData(index, text, Qt::EditRole);
}

void DropDownDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void DropDownDelegate::setDropDownStrings(const QStringList &strings)
{
    m_dropDownStrings = strings;
}
