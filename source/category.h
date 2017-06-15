#ifndef CATEGORY_H
#define CATEGORY_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDomDocument>
#include <QDomElement>

class Category : public QObject
{
    Q_OBJECT
public:
    Category(QObject *parent = nullptr);
    Category(const Category &other);
    ~Category();

    static const QString CategoryTag;
    static const QString IdAttr;
    static const QString NameAttr;
    static const QString StartTimeAttr;

    static const uint InvalidCategoryId = UINT32_MAX;

    Q_PROPERTY(uint m_id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString m_name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QDateTime m_startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)

    uint id() const;
    QString name() const;
    QDateTime startTime() const;

    void setId(const uint id);
    void setName(const QString &name);
    void setStartTime(const QDateTime &startTime);

    void serialise(QDomDocument &xmlDoc, QDomElement &parentNode);

signals:
    void idChanged(const uint newId);
    void nameChanged(const QString &newName);
    void startTimeChanged(const QDateTime &newStartTime);

protected:
    uint m_id;
    QString m_name;
    QDateTime m_startTime;
};

Q_DECLARE_METATYPE(Category)

#endif // CATEGORY_H
