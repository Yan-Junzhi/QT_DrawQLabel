#ifndef CREATEBINANGLE_H
#define CREATEBINANGLE_H

#include <QWidget>
#include <QLabel>


class CreateQlabel : public QLabel
{
    Q_OBJECT
public:
    CreateQlabel(QWidget *parent = nullptr);
    void setLocRotateAngle(qreal angle);
    ~CreateQlabel();
protected:
    void paintEvent(QPaintEvent *event) override;
private:
     qreal _rotateAngle=0.0;
};

class CreateBinAngle : public QWidget
{
    Q_OBJECT
public:
    explicit CreateBinAngle(QWidget *parent = nullptr);
    ~CreateBinAngle();

    qreal angel(){return _angle;}
    void setAngle(qreal angle);
    void setLine(qreal angle,qreal distenceline);
    int distance() const;
    void setDistance(qreal newDistance);
    QPointF getResultPos();
    void mousePressEvent(QMouseEvent *event);
    void setKeep(bool flag);
    void setLocRotateAngle(qreal angle);
    void setApRotateAngle(qreal angle);
protected:
    QPoint getBinPos(qreal angle);
    void moveBin(qreal angle);

    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    CreateQlabel* _bin {Q_NULLPTR};
    qreal _distance = 0;
    qreal _angle = 0;//Loc相对AP的角度
    qreal _angleLine= 0.0;//线的角度
    qreal _rotateAngle=0.0;//Loc旋转的角度
    bool _isPressed = false;
    bool _keep=false;
    QPoint _offset;
    int _limitHeight=600;
    int _limitWidth=400;
    QPointF _rectCenterPos;
    qreal _distanceLine=150.0;
};


#endif // CREATEBINANGLE_H
