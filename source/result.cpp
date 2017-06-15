#include "result.h"

#include <QDomElement>

const QString Result::ResultXmlTag("result");
const QString Result::FinishTimeXmlAttrib("finishTime");
const QString Result::RaceNumberXmlAttrib("raceNumber");


Result::Result(const uint raceNumber, const QDateTime &finishTime)
    : m_raceNumber(raceNumber)
    , m_finishTime(finishTime)
{
    Q_ASSERT(m_finishTime.isValid());
}

uint Result::raceNumber() const
{
    return m_raceNumber;
}

void Result::setRaceNumner(const uint raceNumber)
{
    m_raceNumber = raceNumber;
}

QDateTime Result::finishTime() const
{
    return m_finishTime;
}

void Result::setFinishTime(const QDateTime &finishTime)
{
    m_finishTime = finishTime;
}

void Result::serialise(QDomDocument &xmlDoc, QDomElement &parentNode)
{
    QDomElement el = xmlDoc.createElement(ResultXmlTag);
    el.setAttribute(RaceNumberXmlAttrib, m_raceNumber);
    el.setAttribute(FinishTimeXmlAttrib, m_finishTime.toString(Qt::ISODate));
    parentNode.appendChild(el);
}
