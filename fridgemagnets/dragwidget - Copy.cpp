/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//#include "draglabel.h"
#include "dragwidget.h"
#include <QLabel>
#include <QtWidgets>
#include <QDebug>

static inline QString drapFlag() { return "go"; }

//! [0]
DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent)
{
    m_word_label = new QLabel("test", this);
    m_word_label->move(QPoint(10,10));
    m_word_label->setFixedSize(100, 30);
    setAcceptDrops(true);
    resize(500, 400);
}
/*
  ��ק��ʼ
*/
void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    qDebug() << "dragEnterEvent";
    // �϶��¼�
    if (event->mimeData()->hasFormat(drapFlag())) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
    QWidget::setCursor(Qt::SizeHorCursor);
    qDebug() << "dragEnterEvent ok";
}

/*
  �϶���
*/
void DragWidget::dragMoveEvent(QDragMoveEvent *event)
{
    qDebug() << "dragMoveEvent";

    const QMimeData* mime = event->mimeData();
    // ��ȡ�϶����󴫵ݵ���Ϣ
    QByteArray itemData = mime->data(drapFlag());
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);
    QPoint offset;
    dataStream >> offset;
    // ��ʾ���ƶ�����ǰ����
    m_word_label->setText(QString("x:%1 y:%2").arg(offset.x()).arg(offset.y()));
    m_word_label->move(event->pos() - offset);
    /*
    // �϶�ʱ���ͼ�����ü����϶����
    if (event->mimeData()->hasFormat(drapFlag())) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
    */
    qDebug() << "dragMoveEvent ok";
}

/*
  ��ק����
*/
void DragWidget::dropEvent(QDropEvent *event)
{
    qDebug() << "dropEvent";
    if (event->mimeData()->hasFormat(drapFlag())) {
        /*
        const QMimeData *mime = event->mimeData();
        // ��ȡ�϶����󴫵ݵ���Ϣ
        QByteArray itemData = mime->data(drapFlag());
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPoint offset;
        dataStream >> offset;
        // ��ʾ���ƶ�����ǰ����
        m_word_label->setText(QString("x:%1 y:%2").arg(offset.x()).arg(offset.y()));
        m_word_label->move(event->pos() - offset);
        */
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    }
    QWidget::setCursor(Qt::ArrowCursor);
    qDebug() << "dropEvent ok";
}
/*
  ��������ѹ��ȡ��ǰ����  
*/
void DragWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    // ֻ��ȡ��������ѹ�¼�
    if (!(event->buttons() & Qt::LeftButton)) return;
    // ��ȡ��ҳ����Ϣ���ݸ��϶�����
    auto* child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child) return;
    // ��ȡ����¼��ǰ����
    hotSpot = event->pos() - child->pos();
    qDebug() << "mousePressEvent ok";
}

/*
  ��������ѹʱ�������뵱ǰ����Ա����ƶ������� ��ק����
  ��ס�������϶��ӿؼ�֪�����¸��¼�������
*/
void DragWidget::mouseMoveEvent(QMouseEvent* event)
{
    qDebug() << "mouseMoveEvent";
    if (!(event->buttons() & Qt::LeftButton)) return;

    if ((event->pos() - hotSpot).manhattanLength() 
        < QApplication::startDragDistance())
        return;
    auto* child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child) return;   
    hotSpot = event->pos() - child->pos();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << QPoint(hotSpot);
    QMimeData* mimeData = new QMimeData;
    mimeData->setData(drapFlag(), itemData);
    // �����϶�����
    QDrag* drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(hotSpot);
    drag->exec();
    qDebug() << "mouseMoveEvent ok";
}
