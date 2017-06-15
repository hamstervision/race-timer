#include "category.h"

const QString Category::CategoryTag("category");
const QString Category::IdAttr("id");
const QString Category::NameAttr("name");
const QString Category::StartTimeAttr("startTime");

Category::Category(QObject *parent)
    : QObject(parent)
    , m_id(InvalidCategoryId)
{

}

Category::Category(const Category &other)
{
    m_name = other.name();
    m_startTime = other.startTime();
}

Category::~Category()
{

}

uint Category::id() const
{
    return m_id;
}

QString Category::name() const
{
    return m_name;
}

QDateTime Category::startTime() const
{
    return m_startTime;
}

void Category::setId(const uint id)
{
    m_id = id;
    emit idChanged(m_id);
}

void Category::setName(const QString &name)
{
    m_name = name;
    emit nameChanged(m_name);
}

void Category::setStartTime(const QDateTime &startTime)
{
    m_startTime = startTime;
    emit startTimeChanged(m_startTime);
}

void Category::serialise(QDomDocument &xmlDoc, QDomElement &parentNode)
{
    QDomElement el = xmlDoc.createElement(CategoryTag);
    el.setAttribute(IdAttr, m_id);
    el.setAttribute(NameAttr, m_name);
    el.setAttribute(StartTimeAttr, m_startTime.toString(Qt::ISODate));
    parentNode.appendChild(el);
}
