#ifndef COMPETITOR_H
#define COMPETITOR_H

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QDomElement>

#include "category.h"

class Competitor : public QObject
{
    Q_OBJECT
public:
    explicit Competitor(QObject *parent = nullptr);

    static const QString CompetitorTag;
    static const QString RaceNumberAttr;
    static const QString FirstNameAttr;
    static const QString LastNameAttr;
    static const QString CategoryAttr;
    static const QString MembershipIdAttr;

    Q_PROPERTY(uint m_raceNumber READ raceNumber WRITE setRaceNumber NOTIFY raceNumberChanged)
    Q_PROPERTY(QString m_firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString m_lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(Category* m_category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString m_membershipId READ membershipId WRITE setMembershipId NOTIFY membershipIdChanged)

    uint raceNumber() const;
    QString firstName() const;
    QString lastName() const;
    Category* category() const;
    QString membershipId() const;

    void setRaceNumber(const uint raceNumber);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setCategory(Category *category);
    void setMembershipId(const QString &membershipId);

    void clear();

    void serialise(QDomDocument &xmlDoc, QDomElement &parentNode);

signals:
    void raceNumberChanged(const uint newRaceNumber);
    void firstNameChanged(const QString &newFirstName);
    void lastNameChanged(const QString &newLastName);
    void categoryChanged(Category *newCategory);
    void membershipIdChanged(const QString &newMembershipId);

protected:
    uint m_raceNumber;
    QString m_firstName;
    QString m_lastName;
    Category *m_category;
    QString m_membershipId;
};

#endif // COMPETITOR_H
