#ifndef PAD_H
#define PAD_H

#include <QFrame>
#include <QtWidgets>

namespace Ui {
class pad;
}

class posi{                                                 //测定类
public:
    int l;
    int x;
    posi(QPoint);
    posi(int,int);
    posi();
    void clear();
};

class path{
public:
    QList<int> l;
    QList<int> x;
    QList<int> el;
    QList<int> ex;
    int n;
    path();
    path(int _l, int _x, int _n);
    void clear();
};

class pad : public QFrame
{
    Q_OBJECT

public:
    explicit pad(QWidget *parent = 0);
    ~pad();

private:
    Ui::pad *ui;
    QBrush *sty;
    posi *be;           //起始位置信息
    posi *ta;           //目标位置信息
    int chess[10][5];   //0 blank;1white;2black;3whiteeaten;4blackeaten;5whiteking;6blackking
    QList<path> pa;     //依次绘画路径
    QList<path> allpa;  //全部可走路径
    path cpath;
    int chosen;
    int turn;//2white,1black
    bool st;        //判断是否有选中


    void mousePressEvent(QMouseEvent *ev);

    void paintEvent(QPaintEvent *ev);
    void paintframe(QPainter *p);   //画格子
    void paintchess(QPainter *p);  //画棋子
    void paintpath(QPainter *p);   //画路径
    void act();//移动棋子，发出信号，重新查找路径
    path nextmove(int, int, int);
    QList<path> search();
    QList<path> trans(QList<path>,int,int);

    void win(); //判断胜利
signals:
    void won();
    void here(int);
    void sound(int);
private slots:
    void there(int);
};

#endif // PAD_H
