#ifndef CHART_H
#define CHART_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QMap>
#include <QJsonObject>
/*
    1.获取optiopn
    2.根据option构建不同的图表模板
*/


class Chart
{
public:
    static Chart* instance();
    QJsonObject getOpition(const QVariant&) const;
    void loadOpotions();
private:
    explicit Chart();
private:
    QMap<QVariant,QJsonObject> m_options;
};

#endif // CHART_H
