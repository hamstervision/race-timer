#ifndef RESULT_H
#define RESULT_H

#include <QDateTime>
#include <vector>

class QDomElement;
class QDomDocument;

class Result
{
public:
    static const uint UnknownRaceNumber = 0;

    static const QString ResultXmlTag;
    static const QString FinishTimeXmlAttrib;
    static const QString RaceNumberXmlAttrib;

    Result(const uint raceNumber, const QDateTime &finishTime);

    uint raceNumber() const;
    void setRaceNumner(const uint raceNumber);

    QDateTime finishTime() const;
    void setFinishTime(const QDateTime &finishTime);

    void serialise(QDomDocument &xmlDoc, QDomElement &parentNode);

protected:
    uint m_raceNumber;
    QDateTime m_finishTime;
};

typedef std::vector<Result*> Results;

#endif // RESULT_H
