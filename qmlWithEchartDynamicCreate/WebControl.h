#ifndef WEBCONTROL_H
#define WEBCONTROL_H

#include <QObject>
#include <QVariant>

// test
class WebControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY updateData)
public:
    static WebControl* instance();
    // Q_INVOKABLE 引入当前方法至元对象中，除此之外 signal & slots 也可以被qml识别
    const QVariant& data() const;
    Q_INVOKABLE void setData(const QVariant&);
private:
    explicit WebControl(QObject *parent = nullptr);
    QVariant m_data;

signals:
    void updateData(const QVariant&);
};

#endif // WEBCONTROL_H
