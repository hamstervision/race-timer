#include "categoryitemdelegate.h"

#include <QLineEdit>
#include <QDateTimeEdit>

static const QString RaceTimerDateFormat("yyyy-MM-dd hh:mm:ss");

CategoryItemDelegate::CategoryItemDelegate(CategoriesModel *categoriesModel, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_categoriesModel(categoriesModel)
{
    Q_ASSERT(m_categoriesModel);
}

QString CategoryItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QVariant::DateTime)
    {
        return value.toDateTime().toString(RaceTimerDateFormat);
    }
    return QStyledItemDelegate::displayText(value, locale);
}

QWidget* CategoryItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case CategoriesModel::NameColumnId:
        return new QLineEdit(parent);
    case CategoriesModel::StartTimeColumnId:
        return new QDateTimeEdit(parent);
    default:
        break;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void CategoryItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case CategoriesModel::NameColumnId:
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            if (!lineEdit)
            {
                Q_ASSERT(false);
                return;
            }
            lineEdit->setText(m_categoriesModel->data(index).toString());
        }
        break;
    case CategoriesModel::StartTimeColumnId:
        {
            QDateTimeEdit *timeEdit = qobject_cast<QDateTimeEdit*>(editor);
            if (!timeEdit)
            {
                Q_ASSERT(false);
                return;
            }
            timeEdit->setDisplayFormat(RaceTimerDateFormat);
            timeEdit->setDateTime(m_categoriesModel->data(index).toDateTime());
        }
        break;
    default:
        break;
    }
}

void CategoryItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_ASSERT(model == m_categoriesModel);

    switch (index.column())
    {
    case CategoriesModel::NameColumnId:
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor);
            if (lineEdit)
                m_categoriesModel->setData(index, lineEdit->text());
        }
        break;
    case CategoriesModel::StartTimeColumnId:
        {
            QDateTimeEdit *timeEdit = qobject_cast<QDateTimeEdit*>(editor);
            if (timeEdit)
                m_categoriesModel->setData(index, timeEdit->dateTime());
        }
        break;
    default:
        break;
    }
}

void CategoryItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
