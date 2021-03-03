#include "WebControl.h"


WebControl* WebControl::instance()
{
    static WebControl* obj = new WebControl();
    return obj;
}


WebControl::WebControl(QObject *parent) : QObject(parent)
{

}

const QVariant& WebControl::data() const
{
    return m_data;
}


void WebControl::setData(const QVariant& data)
{
    if(m_data != data)
    {
        m_data = data;
        emit updateData(m_data);
    }
}
