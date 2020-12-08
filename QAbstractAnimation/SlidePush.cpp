#include "SlidePush.h"
#include <QWidget>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QDebug>
#include <QHBoxLayout>
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
SlidePush::SlidePush(
        QWidget* parent,QObject* target,const QByteArray &propertyName,
        RunMode model,double speed)
    :QObject(parent), m_speed(speed), m_pos_mode(model), m_back_state(true)
{
    // 创建位置动画
    m_pos = new QPropertyAnimation(target, "geometry");

    // 初始化
    if (target)init();
    // 事件处理
    connect(m_pos, &QParallelAnimationGroup::finished, [&] {
        if (m_back_state && m_pos->direction() == QAbstractAnimation::Backward)
        {
            QWidget* w = qobject_cast<QWidget*>(m_pos->targetObject());
            if (w) w->hide();
        }
    });
}


////////////////////////////////////////////////////////////////////////////////
void SlidePush::init()
{
    // 设置目标动态参数
    QWidget* target =  qobject_cast<QWidget*>(m_pos->targetObject());
    if(target)
    {
        // 动画周期
        double duration = (m_pos_mode == fromBottom || m_pos_mode == fromTop) ?
                    target->height() : target->width() / m_speed;

        // 位置动画初始化
        m_pos->setDuration(duration);
        QRect start = target->geometry(),end = target->geometry();

        // 根据移动的方向设置动画参数
        if (m_pos_mode == fromBottom)
        {
            start.adjust(0,0,0,-target->height());
            end.adjust(0,-target->height(),0,-target->height());
        }
        else if (m_pos_mode == fromTop)
        {
            start.adjust(0,0,0,-target->height());
            end.adjust(0,0,0,0);
        }
        else if (m_pos_mode == fromLeft)
        {
            start.adjust(0,0,-target->width(),0);
            end.adjust(0,0,0,0);
        }
        else
        {
            start.adjust(0,0,-target->width(),0);
            end.adjust(-target->width(),0,-target->width(),0);
        }
        m_pos->setStartValue(start);
        m_pos->setEndValue(end);
        qDebug() <<"start:" << start <<"end:" << end;
    }
    else
    {
        // ERROR
    }
}


////////////////////////////////////////////////////////////////////////////////
void SlidePush::slidePop()
{
    QWidget* w = qobject_cast<QWidget*>(m_pos->targetObject());
    qDebug() <<"target slide before:" << w->geometry();
    m_back_state = false;
    m_pos->setDirection(QAbstractAnimation::Forward);
    //停止正在进行的动画重新
    if (m_pos->state() == QAbstractAnimation::Running) {
        m_pos->stop();
    }
    m_pos->start();
    w->show();
    qDebug() <<"target slided:" << w->geometry();
}


////////////////////////////////////////////////////////////////////////////////
void SlidePush::slideBack()
{
    m_back_state = true;
    m_pos->setDirection(QAbstractAnimation::Backward);
    //停止正在进行的动画重新
    if (m_pos->state() == QAbstractAnimation::Running) {
        m_pos->stop();
    }
    m_pos->start();
}


////////////////////////////////////////////////////////////////////////////////
void SlidePush::setTargetObject(QObject *target, RunMode m)
{
    m_pos_mode = m;
    m_pos->setTargetObject(target);
    init();
}

////////////////////////////////////////////////////////////////////////////////
void SlidePush::setMode(RunMode m)
{
    m_pos_mode = m;
    QWidget* target = qobject_cast<QWidget*>(m_pos->targetObject());
    if (target)
    {
        const QRect& rec = target->geometry();
        double duration = (m_pos_mode == fromBottom || m_pos_mode == fromTop) ?
                    rec.height() : rec.width() / m_speed;
        m_pos->setDuration(duration);
        const QSize& rectangle = rec.size();
        QPoint start;
        if (m_pos_mode == fromBottom) start = QPoint{ 0,rectangle.height() };
        else if (m_pos_mode == fromTop) start = QPoint{ 0,-rectangle.height() };
        else if (m_pos_mode == fromLeft) start = QPoint{ -rectangle.width(),0 };
        else start = QPoint{ rectangle.width(),0 };
        QPoint end(0, 0);
        m_pos->setStartValue(start);
        m_pos->setEndValue(end);
    }
}


////////////////////////////////////////////////////////////////////////////////
