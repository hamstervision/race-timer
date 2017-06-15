#include "raceresultsitemdelegate.h"

#include <QTime>

static const QString RaceTimerTimeFormat("hh:mm:ss:zzz");

RaceResultsItemDelegate::RaceResultsItemDelegate(RaceResultsModel *model, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_raceResultsModel(model)
{
    Q_ASSERT(m_raceResultsModel);
}

QString RaceResultsItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    if (value.type() == QVariant::LongLong)
    {
        QTime result(0, 0);
        result = result.addMSecs(value.toLongLong());
        return result.toString(RaceTimerTimeFormat);
    }

    return QStyledItemDelegate::displayText(value, locale);
}
