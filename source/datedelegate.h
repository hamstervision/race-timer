#ifndef DATEDELEGATE_H
#define DATEDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTime>
#include <QDateTimeEdit>

static const char RaceTimerDateFormat[] = {"yyyy-MM-dd hh:mm:ss"};

class DateDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:

    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        if (value.type() == QVariant::DateTime)
        {
            return value.toDateTime().toString(RaceTimerDateFormat);
        }
        return QStyledItemDelegate::displayText(value, locale);
     }

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        QWidget *editor = QStyledItemDelegate::createEditor(parent, option, index);
        QDateTimeEdit *dateTimeEdit = dynamic_cast<QDateTimeEdit*>(editor);
        if (dateTimeEdit)
        {
            dateTimeEdit->setDisplayFormat(RaceTimerDateFormat);
        }
        return editor;
    }
};

#endif // DATEDELEGATE_H
