#include "competitor.h"

const QString Competitor::CompetitorTag("competitor");
const QString Competitor::RaceNumberAttr("raceNumber");
const QString Competitor::FirstNameAttr("firstName");
const QString Competitor::LastNameAttr("lastName");
const QString Competitor::CategoryAttr("categoryId");
const QString Competitor::MembershipIdAttr("membershipId");

Competitor::Competitor(QObject *parent)
    : QObject(parent)
    , m_raceNumber(0)
    , m_category(nullptr)
{

}

uint Competitor::raceNumber() const
{
    return m_raceNumber;
}

QString Competitor::firstName() const
{
    return m_firstName;
}

QString Competitor::lastName() const
{
    return m_lastName;
}

Category* Competitor::category() const
{
    return m_category;
}

QString Competitor::membershipId() const
{
    return m_membershipId;
}

void Competitor::setRaceNumber(const uint raceNumber)
{
    m_raceNumber = raceNumber;
    Q_ASSERT(m_raceNumber > 0);
    emit raceNumberChanged(m_raceNumber);
}

void Competitor::setFirstName(const QString &firstName)
{
    m_firstName = firstName;
    emit firstNameChanged(m_firstName);
}

void Competitor::setLastName(const QString &lastName)
{
    m_lastName = lastName;
    emit lastNameChanged(m_lastName);
}

void Competitor::setCategory(Category *category)
{
    m_category = category;
    Q_ASSERT(m_category);
    emit categoryChanged(m_category);
}

void Competitor::setMembershipId(const QString &membershipId)
{
    m_membershipId = membershipId;
    emit membershipIdChanged(m_membershipId);
}

void Competitor::clear()
{
    m_firstName.clear();
    m_lastName.clear();
    m_category = nullptr;
    m_membershipId.clear();
}

void Competitor::serialise(QDomDocument &xmlDoc, QDomElement &parentNode)
{
    QDomElement el = xmlDoc.createElement(CompetitorTag);
    el.setAttribute(RaceNumberAttr, m_raceNumber);
    el.setAttribute(FirstNameAttr, m_firstName);
    el.setAttribute(LastNameAttr, m_lastName);

    if (m_category)
        el.setAttribute(CategoryAttr, m_category->id());

    if (!m_membershipId.isEmpty())
        el.setAttribute(MembershipIdAttr, m_membershipId);

    parentNode.appendChild(el);
}
