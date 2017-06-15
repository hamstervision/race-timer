#ifndef RACERESULTSITEMDELEGATE_H
#define RACERESULTSITEMDELEGATE_H

#include <QStyledItemDelegate>
#include "raceresultsmodel.h"

class RaceResultsItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RaceResultsItemDelegate(RaceResultsModel *model, QObject *parent = nullptr);

public: // QStyledItemDelegate
    QString displayText(const QVariant &value, const QLocale &locale) const override;

protected:
    RaceResultsModel *m_raceResultsModel;
};


#endif // RACERESULTSITEMDELEGATE_H
