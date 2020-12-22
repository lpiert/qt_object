#ifndef WEBCONTROL_H
#define WEBCONTROL_H

#include <QObject>
#include <QVariant>

class WebControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant style READ style WRITE setStyle NOTIFY updateStyle)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY updateData)
public:
    static WebControl* instance();
    const QVariant& style() const;
    // Q_INVOKABLE 引入当前方法至元对象中，除此之外 signal & slots 也可以被qml识别
    Q_INVOKABLE void setStyle(const QVariant&);
    const QVariant& data() const;
    Q_INVOKABLE void setData(const QVariant&);
private:
    explicit WebControl(QObject *parent = nullptr);
    QVariant m_style;
    QVariant m_data;
signals:
    void updateStyle(const QVariant&);
    void updateData(const QVariant&);
};

#endif // WEBCONTROL_H
