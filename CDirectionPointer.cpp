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

#include "CDirectionPointer.h"

#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QDebug>

CDirectionPointer::CDirectionPointer(QWidget *parent)
    : QWidget(parent)
{ 
    this->setMouseTracking(true);
}

void CDirectionPointer::paintEvent(QPaintEvent *)
{
    int side = qMin(width(), height());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //    //换算中心点
    //    painter.translate(width() / 2, height() / 2);
    painter.setPen(QColor(0,170,255));
    QPoint center = QPoint(width() / 2, height() / 2);
    _ellipseCenterPos = center;
    int r = (side -12.0)/2.0;
    painter.drawEllipse(center,r,r);
    QLineF lineF(center,QPoint(center.x()+r,center.y()));
    qreal tAngle = angle();
    lineF.setAngle(tAngle);
    painter.drawLine(lineF);
    if(_isHovered){
        painter.setBrush(QBrush(QColor(0,255,255)));
    }else{
        painter.setBrush(QBrush(QColor(255,0,255)));
    }
    _rect = QRect(lineF.p2().x() - 6,lineF.p2().y() - 6,12,12);
    painter.drawEllipse(_rect);
    //    //标记刻度
    painter.drawLine(QPoint(center.x()+r-5,center.y()),QPoint(center.x()+r,center.y()));
    painter.drawText(QPoint(center.x()+r+5,center.y()+4),QStringLiteral("0°"));

    painter.drawLine(QPoint(center.x()-r+5,center.y()),QPoint(center.x()-r,center.y()));
    painter.drawText(QPoint(center.x()-r-35,center.y()+4),QStringLiteral("-180°"));
}

void CDirectionPointer::mousePressEvent(QMouseEvent *event)
{
    QRegion region(_rect,QRegion::Ellipse);
    if(region.contains(event->pos())){
        _isPressed = true;
    }
    qDebug()<<_angle;
    QWidget::mousePressEvent(event);
}
void CDirectionPointer::mouseMoveEvent(QMouseEvent *event)
{
    QRegion region(_rect,QRegion::Ellipse);
    bool isHovered = false;
    if(region.contains(event->pos())){
        isHovered = true;
    }
    if(_isHovered != isHovered){
        _isHovered = isHovered;
        this->update();
    }

    if(_isPressed){
        QLineF lineF(_ellipseCenterPos,event->pos());
        qreal angle = lineF.angle();
        if(angle>180){
            angle = -(360 - angle);
        }
        setAngle(angle);
        emit sigAngleChanged(angle);
    }
    qDebug()<<_angle;
    QWidget::mouseMoveEvent(event);
}
void CDirectionPointer::mouseReleaseEvent(QMouseEvent *event)
{
    _isPressed = false;
    QWidget::mouseReleaseEvent(event);
}

qreal CDirectionPointer::angle() const
{
    return _angle;
}

void CDirectionPointer::setAngle(const qreal &angle)
{
    _angle = angle;
    this->update();
}
