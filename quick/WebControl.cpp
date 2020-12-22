#include "WebControl.h"


WebControl* WebControl::instance()
{
    static WebControl* obj = new WebControl();
    return obj;
}


WebControl::WebControl(QObject *parent) : QObject(parent)
{

}


const QVariant& WebControl::style() const
{
    return m_style;
}


void WebControl::setStyle(const QVariant& style)
{
    if(m_style != style)
    {
        m_style = style;
        emit updateStyle(m_style);
    }
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
