#include "CreateBinAngle.h"

#include <QtMath>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#define TRIANGLE_WIDTH 15               // 小三角的宽度;
#define TRIANGLE_HEIGHT 10              // 小三角的高度;

CreateQlabel::CreateQlabel(QWidget *parent) : QLabel(parent)
{

}

void CreateQlabel::setLocRotateAngle(qreal angle)
{
    _rotateAngle=angle;
    this->update();
}

CreateQlabel::~CreateQlabel()
{

}

void CreateQlabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    QBrush brush;
    //    setStyleSheet("background-color:rgb(222,238,246); border:2px solid rgb(0,0,0); color:rgb(45,45,45);");
    painter.translate(this->rect().center().x(),this->rect().center().y());
    painter.rotate(-_rotateAngle);
    painter.translate(-this->rect().center().x(), -this->rect().center().y());//原点回复

    painter.setPen(QPen(Qt::black,2));
    painter.drawRect(this->rect().center().x()-30,this->rect().center().y()-30,60,60);
    painter.setPen(QPen(QColor(45,45,45),2));
    painter.drawText(rect(),Qt::AlignCenter,"Loc");

    QPolygonF polygonF;
    polygonF<<QPointF(rect().center().x()+20,rect().center().y()+10)<<QPointF(rect().center().x()+20,rect().center().y()-10)<<QPointF(rect().center().x()+30,rect().center().y());
    painter.setBrush(Qt::red);
    painter.setPen(Qt::red);
    painter.drawPolygon(polygonF, Qt::WindingFill);
}

CreateBinAngle::CreateBinAngle(QWidget *parent) : QWidget(parent)
{

    _bin = new CreateQlabel(this);
    _bin->resize(40,40);

}

CreateBinAngle::~CreateBinAngle()
{
    if(_bin) delete _bin;
}

void CreateBinAngle::setKeep(bool flag)
{
    _keep=flag;
    _bin->setLocRotateAngle(_rotateAngle);

    this->update();
}

void CreateBinAngle::setLocRotateAngle(qreal angle)
{
    _bin->setLocRotateAngle(angle);//LOC的旋转角
    this->update();
}

void CreateBinAngle::setApRotateAngle(qreal angle)
{
    _rotateAngle=angle;//AP的旋转角
    if(_keep)setLocRotateAngle(angle);
    this->update();
}

void CreateBinAngle::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    moveBin(0);
}

int CreateBinAngle::distance() const
{
    return _distance;
}

void CreateBinAngle::setDistance(qreal newDistance)
{
    _distance = newDistance;
}

void CreateBinAngle::setAngle(qreal angle)
{
    _angle = angle;
    _angleLine = angle;
    moveBin(_angle);
    QPoint pBin = getBinPos(_angle);
    QLineF lineF(this->rect().center(), pBin + _bin->rect().center());
    setLine(angle,lineF.length());
    this->update();
}

void CreateBinAngle::setLine(qreal angle,qreal distenceline)
{
    if(angle>180){
        angle = -(360 - angle);
    }
    _angleLine=angle;
    //    _bin->setLocRotateAngle(angle);//跟连线平行
    _distanceLine=distenceline;
    this->update();
}
void CreateBinAngle::moveBin(qreal angle)
{
    _bin->move( getBinPos(angle) );
}

QPointF CreateBinAngle::getResultPos()
{
    qreal distance = _distance*100;
    qreal degree = qDegreesToRadians(_angle);//旋转角度和站点一致
    QPointF pRotate( distance*(qCos(degree)), distance*(qSin(degree)) );
    return pRotate;
}
QPoint CreateBinAngle::getBinPos(qreal angle)
{
    int dis = 3;
    qreal distance = qMin(width()/dis,height()/dis);
    qreal degree = -qDegreesToRadians(_angle);
    QPoint pRotate( distance*(qCos(degree)), distance*(qSin(degree)) );
    QPoint pos = pRotate + this->rect().center() - _bin->rect().center();
    return pos;
}
void CreateBinAngle::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int size = qMin(width()/6,height()/6);
    //resize Bin
    _bin->resize(size,size);
    moveBin(_angle);
}

void CreateBinAngle::paintEvent(QPaintEvent *event)
{
    int size = qMin(width()/6,height()/6);

    QPoint pBin = getBinPos(_angle);

    //draw others
    QPainter painter;
    painter.begin(this);
    {
        QPen pen;
        //draw LM

        //draw line
        pen.setStyle(Qt::PenStyle::DotLine);
        pen.setBrush(QColor(132,223,223));
        pen.setWidth(4);
        painter.setPen(pen);
        QLineF lineF(this->rect().center(), pBin + _bin->rect().center());
        qreal tAngle = _angleLine;
        qreal tLength= _distanceLine;
        lineF.setAngle(tAngle);
        lineF.setLength(tLength);
        painter.drawLine(lineF);
        _rectCenterPos = rect().center();

        painter.translate(this->rect().center().x(),this->rect().center().y());
        painter.rotate(-_rotateAngle);
        painter.translate(-this->rect().center().x(), -this->rect().center().y());//原点回复

        pen.setWidth(2);
        pen.setStyle(Qt::PenStyle::DashLine);
        pen.setBrush(QColor(65,159,65));
        painter.setPen(pen);
        QRectF fillrect(this->rect().center().x()-size/2,this->rect().center().y()-size/2,size,size);
        painter.fillRect(fillrect,QColor(190,249,73));
        painter.drawRect(fillrect);

        //draw center
        pen.setStyle(Qt::PenStyle::SolidLine);
        pen.setBrush(QColor(250,68,49));
        painter.setPen(pen);
        painter.drawLine(rect().center()+QPoint(-10,0),rect().center()+QPoint(10,0));
        painter.drawLine(rect().center()+QPoint(0,-10),rect().center()+QPoint(0,10));

        //draw letter LM
        pen.setColor(QColor(45,45,45));
        pen.setStyle(Qt::PenStyle::SolidLine);
        //        pen.setBrush(QColor(190,249,73));
        painter.setFont(QFont("Microsoft Yahei", 12,60));
        painter.setPen(pen);
        painter.drawText(rect().center()+QPoint(-16,9),"AP");


        //draw red triangle
        QPolygonF polygonF;//多边形
        painter.setBrush(Qt::red);
        painter.setPen(Qt::red);
        QPointF point1(rect().center().x()+30,rect().center().y()+10);
        polygonF.append(point1);
        QPointF point2(rect().center().x()+30,rect().center().y()-10);
        polygonF.append(point2);
        QPointF point3(rect().center().x()+40,rect().center().y());
        polygonF.append(point3);
        painter.drawPolygon(polygonF);

    }
    painter.end();

}

void CreateBinAngle::mousePressEvent(QMouseEvent *event)
{

    auto prePos = event->pos();

    if(_bin->geometry().contains(prePos)){
        _isPressed = true;
        _offset=prePos-_bin->geometry().topLeft();
    }
    // qDebug()<<prePos<< _isPressed;
    QWidget::mousePressEvent(event);
}

void CreateBinAngle::mouseMoveEvent(QMouseEvent *event)
{
    auto point = event->pos();
    if(_isPressed){
        QPoint pBin = getBinPos(_angle);
        /*        qreal x = _bin->geometry().center().x();
        qreal y = _bin->geometry().center().y();
        bool isIgnore = false;
        //左边
        if(x< _bin->width()/2.0){
            x =0;
            isIgnore = true;
        }
        //右边
        if(x>_limitWidth - _bin->width() /2.0){
            x =_limitWidth - _bin->width();
            isIgnore = true;
        }
        //上边
        if(y< _bin->height()/2.0){
            y = 0;
            isIgnore = true;
        }
        //下边
        if(y> _limitHeight - _bin->height()/2.0){
            y = _limitHeight - _bin->height();
            isIgnore = true;
        }//画框限位（不能离开图片所在区域）
        if(isIgnore){
            event->ignore();
            auto scenePos = QPointF(x,y);
            _bin->move(x,y);//设置Item左上角位置在图片中的位置（Item的parent是图片）
            return ;
        }*/
        QLineF lineF(_rectCenterPos,_bin->geometry().center());
        setLine(lineF.angle(),lineF.length());
        _bin->move(point-_offset);

    }
    qDebug()<<_angleLine ;
    //    qDebug()<<this->rect().center();
    QWidget::mouseMoveEvent(event);
}
void CreateBinAngle::mouseReleaseEvent(QMouseEvent *event)
{
    _isPressed = false;
    QWidget::mouseReleaseEvent(event);
}
