#include "racetimingitemdelegate.h"

#include <QLineEdit>
#include <QDateTimeEdit>
#include <QSpinBox>

#include <climits>

static const QString RaceTimerTimeFormat("hh:mm:ss");


RaceTimingItemDelegate::RaceTimingItemDelegate(RaceTimingModel *model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_raceTimingModel(model)
{
    Q_ASSERT(m_raceTimingModel);
}

QString RaceTimingItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QVariant::DateTime)
    {
        return value.toDateTime().toString(RaceTimerTimeFormat);
    }
    return QStyledItemDelegate::displayText(value, locale);
}

QWidget *RaceTimingItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case RaceTimingModel::TimeColumnId:
        return new QDateTimeEdit(parent);
    case RaceTimingModel::RaceNumberColumnId:
        return new QSpinBox(parent);
    case RaceTimingModel::NameColumnId:
        return new QLineEdit(parent);
    default:
        break;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void RaceTimingItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch (index.column())
    {
    case RaceTimingModel::TimeColumnId:
        {
            QDateTimeEdit *timeEdit = qobject_cast<QDateTimeEdit*>(editor);
            if (!timeEdit)
            {
                Q_ASSERT(false);
                return;
            }
            timeEdit->setDisplayFormat(RaceTimerTimeFormat);
            timeEdit->setDateTime(m_raceTimingModel->data(index).toDateTime());
        }
        break;
    case RaceTimingModel::RaceNumberColumnId:
        {
            QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);
            if (!spinBox)
            {
                Q_ASSERT(false);
                return;
            }
            spinBox->setRange(0, INT_MAX);
            spinBox->setValue(m_raceTimingModel->data(index).toInt());
        }
        break;
    default:
        break;
    }
}

void RaceTimingItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    Q_ASSERT(model == m_raceTimingModel);

    switch (index.column())
    {
    case RaceTimingModel::TimeColumnId:
        {
            QDateTimeEdit *timeEdit = qobject_cast<QDateTimeEdit*>(editor);
            if (timeEdit)
                m_raceTimingModel->setData(index, timeEdit->dateTime());
        }
        break;
    case RaceTimingModel::RaceNumberColumnId:
        {
            QSpinBox *numberEdit = qobject_cast<QSpinBox*>(editor);
            if (numberEdit)
                m_raceTimingModel->setData(index, numberEdit->value());
        }
        break;
    default:
        break;
    }
}

void RaceTimingItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
