#include "pad.h"
#include "ui_pad.h"

pad::pad(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::pad)
{
    be=new posi();
    ta=new posi();
    cpath.clear();
    ui->setupUi(this);
    for(int i=0;i<4;i++)
        for(int j=0;j<5;j++)
            chess[i][j]=1;
    for(int i=4;i<6;i++)
        for(int j=0;j<5;j++)
            chess[i][j]=0;
    for(int i=6;i<10;i++)
        for(int j=0;j<5;j++)
            chess[i][j]=2;
    /*chess[1][1]=1;
    chess[3][1]=1;
    chess[5][1]=1;
    chess[7][1]=1;
    chess[6][2]=1;
    chess[2][4]=1;
    chess[1][3]=2;
    chess[1][4]=2;
    chess[4][1]=2;
    chess[8][2]=2;

    chess[0][0]=1;*/

    chosen=0;
    turn=1;
    allpa=search();
    pa=allpa;
    qDebug()<<"allpa"<<allpa.size();
}

pad::~pad()
{
    delete ui;
}

void pad::mousePressEvent(QMouseEvent *ev){
    QPoint pnt=ev->pos();
    posi *tem=new posi(pnt);
    bool ista=false;
    qDebug()<<tem->l<<tem->x;
    if(!pa.isEmpty()){//判断是否正确点击在终点位置
        for(int i=0;i<pa.size();i++){
            if(pa[i].l.last()==tem->l&&pa[i].x.last()==tem->x){
                ista=true;
                break;
            }
        }
    }
    qDebug()<<"ista"<<ista;
    if(tem->x==5||chess[tem->l][tem->x]==0){//选中空位
        if(!st||(st&&!ista)){//点空
            st=false;
            pa=allpa;
            be->clear();
            ta->clear();
            update();
            qDebug()<<"mouse1";
            return;
        }else{//点对
            ta=tem;
            pa.clear(); //不再绘画路径
            for(int i=0;i<allpa.size();i++){
                if(allpa[i].l.first()==be->l&&allpa[i].x.first()==be->x&&allpa[i].l.last()==ta->l&&allpa[i].x.last()==ta->x){
                    cpath=allpa[i];//当前路径及编号
                    chosen=i;
                    break;
                }
            }
            st=false;
            act();
            emit here(chosen);
            win();
            qDebug()<<"mouse3";
            be->clear();
            ta->clear();
            cpath.clear();
            chosen=0;
        }
    }
    if(chess[tem->l][tem->x]==((turn==1)?2:1)||chess[tem->l][tem->x]==((turn==1)?2:1)+4){//选中己方
        be= tem;
        ta->clear();
        pa.clear();
        for(int i=0;i<allpa.size();i++){
            if(allpa[i].l.first()==be->l&&allpa[i].x.first()==be->x)
                pa.append(allpa[i]);
        }
        st=true;
        update();
        qDebug()<<"mouse2";
    }
    if(chess[tem->l][tem->x]==turn||chess[tem->l][tem->x]==turn+4){//选中对方
        st=false;
        pa=allpa;
        be->clear();
        ta->clear();
        update();
        qDebug()<<"mouse1";
        return;
    }
}

void pad::paintEvent(QPaintEvent *ev){
    QPainter *p=new QPainter(this);
    p->save();
    paintframe(p);
    paintchess(p);
    paintpath(p);
    p->restore();
}

void pad::paintframe(QPainter *p){
    QColor whitegrid(255,248,220);
    QColor blackgrid(205,133,63);
    //p->setPen(Qt::NoPen);
    for(int i=2;i<502;i=i+50)
        for(int j=2;j<502;j=j+50){
            if((((i-2)/50)%2)==0){//偶数行
                if((((j-2)/50)%2)==0)//偶数列
                    p->setBrush(QBrush(whitegrid));
                else
                    p->setBrush(QBrush(blackgrid));
                QRectF rectangle(j,i,50,50);
                p->drawRect(rectangle);
            }else{
                if((((j-2)/50)%2)==1)//奇数列
                    p->setBrush(QBrush(whitegrid));
                else
                    p->setBrush(QBrush(blackgrid));
                p->drawRect(j,i,50,50);
            }
        }
}

void pad::paintchess(QPainter *p){
    sty=new QBrush();
    QColor whitechess(Qt::black);
    QColor blackchess(240,240,240);
    QPen pen(Qt::red,4);

    for(int i=0;i<10;i++)
        for(int j=0;j<5;j++){
            switch (chess[i][j]) {
            case 1:
                sty->setColor(whitechess);
                sty->setStyle(Qt::SolidPattern);
                break;
            case 2:
                sty->setColor(blackchess);
                sty->setStyle(Qt::SolidPattern);
                break;
            case 3:
                sty->setColor(whitechess);
                sty->setStyle(Qt::Dense3Pattern);
                break;
            case 4:
                sty->setColor(blackchess);
                sty->setStyle(Qt::Dense3Pattern);
                break;
            case 5:
                sty->setColor(whitechess);
                sty->setStyle(Qt::SolidPattern);
                break;
            case 6:
                sty->setColor(blackchess);
                sty->setStyle(Qt::SolidPattern);
                break;
            default:
                continue;
            }
            p->save();
            if((i%2)==0)
                p->translate((52+j*100),(2+i*50));
            else
                p->translate((2+j*100),(2+i*50));
            p->setBrush(*sty);
            p->setPen(Qt::NoPen);
            p->drawEllipse(4,4,42,42);
            if(chess[i][j]>=5){
                QFont f("Segoe Script");
                f.setPixelSize(30);
                f.setBold(true);
                p->setFont(f);
                if(chess[i][j]==5)
                    p->setPen(QPen(blackchess));
                else
                    p->setPen(QPen(whitechess));
                p->drawText(12,38,"K");
            }
            if(st&&j==be->x&&i==be->l){
                p->setBrush(Qt::NoBrush);
                p->setPen(pen);
                p->drawEllipse(3, 3.8, 44, 44);
            }
            p->restore();
        }
}

void pad::paintpath(QPainter *p){
    if(pa.isEmpty())
        return;
    sty=new QBrush();
    QColor pencolor("#FFD700");
    QPen pen(pencolor,4);
    pen.setStyle(Qt::DashLine);
    QColor white(0,0,0,0);
    QColor red(255,0,0,180);
    for(int i=0;i<pa.size();i++){
        //画目标格子
        int y=pa[i].l.last()*50+2;
        int x;
        if(pa[i].l.last()%2==0)
            x=pa[i].x.last()*100+50+2;
        else
            x=pa[i].x.last()*100+2;
        QRadialGradient radialGradient(QPointF(x+25, y+25), 25);
        radialGradient.setColorAt(0, white);
        radialGradient.setColorAt(1, red);
        p->setBrush(radialGradient);
        p->setPen(Qt::NoPen);
        p->drawRect(x,y,50,50);

        if(pa[i].n!=0){//如果有吃子
            for(int j=1;j<pa[i].l.size();j++){
                int y1=pa[i].l[j-1]*50+2+25;
                int y2=pa[i].l[j]*50+2+25;
                int x1;
                int x2;
                if(pa[i].l[j-1]%2==0)
                    x1=pa[i].x[j-1]*100+50+2+25;
                else
                    x1=pa[i].x[j-1]*100+2+25;
                if(pa[i].l[j]%2==0)
                    x2=pa[i].x[j]*100+50+2+25;
                else
                    x2=pa[i].x[j]*100+2+25;
                p->setPen(pen);
                p->setBrush(Qt::NoBrush);
                p->drawLine(x1,y1,x2,y2);
            }
        }
    }

}

void pad::act(){
    if(cpath.n==0){
        chess[cpath.l.last()][cpath.x.last()]=chess[cpath.l.first()][cpath.x.first()];
        chess[cpath.l.first()][cpath.x.first()]=0;
        emit sound(4);
        update();
    }else{
        for(int i=0;i<cpath.el.size();i++){
            chess[cpath.el[i]][cpath.ex[i]]=chess[cpath.el[i]][cpath.ex[i]]+((chess[cpath.el[i]][cpath.ex[i]]<=2)?2:-2);
            chess[cpath.l[i+1]][cpath.x[i+1]]=chess[cpath.l[i]][cpath.x[i]];
            chess[cpath.l[i]][cpath.x[i]]=0;
            emit sound(4);
            update();
            QEventLoop eventloop;
            QTimer::singleShot(500, &eventloop, SLOT(quit())); //wait 0.5s
            eventloop.exec();
        }
    }
    for(int i=0;i<10;i++)
        for(int j=0;j<5;j++){
            if(chess[i][j]==3||chess[i][j]==4)
                chess[i][j]=0;
            if((i==0&&chess[i][j]==2)||(i==9&&chess[i][j]==1))
                chess[i][j]=chess[i][j]+4;
        }
    turn=(turn==1)?2:1;
    allpa=search();
    pa=allpa;
}

QList<path> pad::search(){
    QList<path> temp;
    temp.clear();
    for(int i=0;i<10;i++)
        for(int j=0;j<5;j++){
            if(chess[i][j]==turn||chess[i][j]==turn+4||chess[i][j]==0)
                continue;
            QList<path> b;
            b.clear();
            qDebug()<<i<<j;
            QList<path> c=trans(b,i,j);
            qDebug()<<"c"<<c.size();
            if(c.isEmpty())
                continue;
            if(temp.isEmpty())
                temp=c;
            else{
                if(c.first().n>temp.first().n)
                    temp=c;
                else if(c.first().n==temp.first().n)
                    temp.append(c);
                else
                    continue;
            }
        }
    return temp;
}

QList<path> pad::trans(QList<path> t,int _l,int _x){
    QList<path> temp=t;

    int ud=(turn==2)?1:-1;//白的向下走，黑的向上走
    int left=(_l%2==0)?0:-1;//偶数行走左x-0,奇数行走左x-1
    int right=(_l%2==0)?1:0;//偶数行走右x+1,奇数行走有x+0

    bool em=true;//是否初次移动
    bool change=false;//是否改变

    bool king=false;
    if(chess[_l][_x]>=5)
        king=true;

    path _t(_l,_x,0);
    if(!t.isEmpty()){
        qDebug()<<"#"<<temp.size();
        _t=temp.takeLast();
        em=false;
    }
    path _q=_t;

    if(!king){
        //白:吃左下；黑吃左上
        if((_l+ud*2>=0)&&(_l+ud*2<=9)&&(_x-1>=0)&&chess[_l+ud][_x+left]==turn&&chess[_l+ud*2][_x-1]==0){
            _t.l.append(_l+ud*2);
            _t.x.append(_x-1);
            _t.el.append(_l+ud);
            _t.ex.append(_x+left);
            _t.n++;
            chess[_l][_x]=0;
            chess[_l+ud][_x+left]=(turn==2)?4:3;
            chess[_l+ud*2][_x-1]=(turn==2)?1:2;
            QList<path> p;
            p.clear();
            p.append(_t);
            p=trans(p,_l+ud*2,_x-1);
            temp=p;
            chess[_l+ud][_x+left]=turn;
            chess[_l+ud*2][_x-1]=0;
            chess[_l][_x]=(turn==2)?1:2;
            _t=_q;
            change=true;
            qDebug()<<"eatleftnext"<<_l+ud*2;
            qDebug()<<"*"<<temp.size();
        }//白:吃右下；黑吃右上
        if((_l+ud*2>=0)&&(_l+ud*2<=9)&&(_x+1<=4)&&chess[_l+ud][_x+right]==turn&&chess[_l+ud*2][_x+1]==0){
            _t.l.append(_l+ud*2);
            _t.x.append(_x+1);
            _t.el.append(_l+ud);
            _t.ex.append(_x+right);
            _t.n++;
            chess[_l][_x]=0;
            chess[_l+ud][_x+right]=(turn==2)?4:3;
            chess[_l+ud*2][_x+1]=(turn==2)?1:2;
            QList<path> p;
            p.clear();
            p.append(_t);
            p=trans(p,_l+ud*2,_x+1);
            if(!temp.isEmpty()){
                if(p.last().n==temp.last().n)
                    temp.append(p);
                if(p.last().n>temp.last().n)
                    temp=p;
            }else
                temp=p;
            chess[_l+ud][_x+right]=turn;
            chess[_l+ud*2][_x+1]=0;
            chess[_l][_x]=(turn==2)?1:2;
            _t=_q;
            change=true;
            qDebug()<<"eatrightnext";
        }//白:吃左上；黑吃左上
        if((_l-ud*2>=0)&&(_l-ud*2<=9)&&(_x-1>=0)&&chess[_l-ud][_x+left]==turn&&chess[_l-ud*2][_x-1]==0){
            _t.l.append(_l-ud*2);
            _t.x.append(_x-1);
            _t.el.append(_l-ud);
            _t.ex.append(_x+left);
            _t.n++;
            chess[_l][_x]=0;
            chess[_l-ud][_x+left]=(turn==2)?4:3;
            chess[_l-ud*2][_x-1]=(turn==2)?1:2;
            QList<path> p;
            p.clear();
            p.append(_t);
            p=trans(p,_l-ud*2,_x-1);
            if(!temp.isEmpty()){
                if(p.last().n==temp.last().n)
                    temp.append(p);
                if(p.last().n>temp.last().n)
                    temp=p;
            }else
                temp=p;
            chess[_l-ud][_x+left]=turn;
            chess[_l-ud*2][_x-1]=0;
            chess[_l][_x]=(turn==2)?1:2;
            _t=_q;
            change=true;
            qDebug()<<"eatleftlast";
        }//白:吃右上；黑吃右下
        if((_l-ud*2>=0)&&(_l-ud*2<=9)&&(_x+1<=4)&&chess[_l-ud][_x+right]==turn&&chess[_l-ud*2][_x+1]==0){
            _t.l.append(_l-ud*2);
            _t.x.append(_x+1);
            _t.el.append(_l-ud);
            _t.ex.append(_x+right);
            _t.n++;
            chess[_l][_x]=0;
            chess[_l-ud][_x+right]=(turn==2)?4:3;
            chess[_l-ud*2][_x+1]=(turn==2)?1:2;
            QList<path> p;
            p.clear();
            p.append(_t);
            p=trans(p,_l-ud*2,_x+1);
            if(!temp.isEmpty()){
                if(p.last().n==temp.last().n)
                    temp.append(p);
                if(p.last().n>temp.last().n)
                    temp=p;
            }else
                temp=p;
            chess[_l-ud][_x+right]=turn;
            chess[_l-ud*2][_x+1]=0;
            chess[_l][_x]=(turn==2)?1:2;
            _t=_q;
            change=true;
            qDebug()<<"eatrightlast";
        }
        if(!em&&!change){
            temp.append(_t);
        }
        if(em&&temp.isEmpty()){//如果不处于初次移动则不可不吃子移动，若仍未移动，说明无棋可吃
            qDebug()<<"tempempty";
            //白：左下；黑：左上
            if((_l+ud>=0)&&(_l+ud<=9)&&(_x+left>=0)&&chess[_l+ud][_x+left]==0){
                _t.l.append(_l+ud);
                _t.x.append(_x+left);
                temp.append(_t);
                _t=_q;
                qDebug()<<"emptyleft";
            }//白：右下；黑：右上
            if((_l+ud>=0)&&(_l+ud<=9)&&(_x+right<=4)&&chess[_l+ud][_x+right]==0){
                _t.l.append(_l+ud);
                _t.x.append(_x+right);
                temp.append(_t);
                _t=_q;
                qDebug()<<"emptyright";
            }
        }
    }else{
        for(int k=1;k<=4;k++){
            path tpath=nextmove(k,_l,_x);
            if(tpath.l.size()!=0){
                if(tpath.n==0&&!em)
                    continue;
                qDebug()<<"&&&&&&&&"<<k;
                for(int i=0;i<tpath.l.size();i++){
                    qDebug()<<"******"<<i;
                    _t.l.append(tpath.l[i]);
                    qDebug()<<tpath.l[i];
                    _t.x.append(tpath.x[i]);
                    _t.n=_t.n+tpath.n;
                    qDebug()<<tpath.n<<_t.n;
                    chess[_l][_x]=0;
                    if(tpath.el.size()!=0){
                        for(int j=0;j<tpath.el.size();j++){
                            _t.el.append(tpath.el[j]);
                            _t.ex.append(tpath.ex[j]);
                            chess[tpath.el[j]][tpath.ex[j]]=(turn==2)?4:3;
                        }
                    }
                    chess[tpath.l[i]][tpath.x[i]]=(turn==2)?5:6;
                    QList<path> p;
                    p.clear();
                    p.append(_t);
                    if(!(em&&_t.n==0))
                        p=trans(p,tpath.l[i],tpath.x[i]);
                    if(!temp.isEmpty()){
                        if(p.last().n==temp.last().n)
                            temp.append(p);
                        if(p.last().n>temp.last().n)
                            temp=p;
                    }else
                        temp=p;
                    if(tpath.el.size()!=0){
                        for(int j=0;j<tpath.el.size();j++)
                            chess[tpath.el[j]][tpath.ex[j]]=turn;
                    }
                    chess[tpath.l[i]][tpath.x[i]]=0;
                    chess[_l][_x]=(turn==2)?5:6;
                    change=true;
                    _t=_q;
                }
            }
        }
        if(!em&&!change){
            temp.append(_t);
        }
    }
    return temp;
}

path pad::nextmove(int type, int a, int b){
    int _l=a;
    int _x=b;
    path temp;
    if(type==1){//往左下
        while((_l+1<=9)&&(_x-((_l%2==0)?0:1)>=0)){
            int dx=_x;
            _x=_x-((_l%2==0)?0:1);
            _l=_l+1;
            if(chess[_l][_x]==0){
                temp.l.append(_l);
                temp.x.append(_x);
            }else if(chess[_l][_x]==turn){
                if(_l+1<=9&&dx-1>=0&&chess[_l+1][dx-1]==0){
                    temp.l.clear();
                    temp.x.clear();
                    temp.el.append(_l);
                    temp.ex.append(_x);
                    temp.n++;
                }
            }else{
                break;
            }
        }
        return temp;
    }
    if(type==2){//往右下
        while((_l+1<=9)&&(_x+((_l%2==1)?0:1)<=4)){
            int dx=_x;
            _x=_x+((_l%2==1)?0:1);
            _l=_l+1;
            if(chess[_l][_x]==0){
                temp.l.append(_l);
                temp.x.append(_x);
            }else if(chess[_l][_x]==turn){
                if(_l+1<=9&&dx+1<=4&&chess[_l+1][dx+1]==0){
                    temp.l.clear();
                    temp.x.clear();
                    temp.el.append(_l);
                    temp.ex.append(_x);
                    temp.n++;
                }
            }else{
                break;
            }
        }
        return temp;
    }
    if(type==3){//往左上
        while((_l-1>=0)&&(_x-((_l%2==0)?0:1)>=0)){
            int dx=_x;
            _x=_x-((_l%2==0)?0:1);
            _l=_l-1;
            if(chess[_l][_x]==0){
                temp.l.append(_l);
                temp.x.append(_x);
            }else if(chess[_l][_x]==turn){
                if(_l-1>=0&&dx-1>=0&&chess[_l-1][dx-1]==0){
                    temp.l.clear();
                    temp.x.clear();
                    temp.el.append(_l);
                    temp.ex.append(_x);
                    temp.n++;
                }
            }else{
                break;
            }
        }
        return temp;
    }
    if(type==4){//往右上
        while((_l-1>=0)&&(_x+((_l%2==1)?0:1)<=4)){
            int dx=_x;
            _x=_x+((_l%2==1)?0:1);
            _l=_l-1;
            if(chess[_l][_x]==0){
                temp.l.append(_l);
                temp.x.append(_x);
            }else if(chess[_l][_x]==turn){
                if(_l-1>=0&&dx+1<=4&&chess[_l-1][dx+1]==0){
                    temp.l.clear();
                    temp.x.clear();
                    temp.el.append(_l);
                    temp.ex.append(_x);
                    temp.n++;
                }
            }else{
                break;
            }
        }
        return temp;
    }
}

void pad::there(int a){
    if(a>=1000)
        return;
    chosen=a;
    pa.clear(); //不再绘画路径
    cpath=allpa[chosen];//当前路径及编号
    st=false;
    act();
    qDebug()<<"there";
    win();
    be->clear();
    ta->clear();
    cpath.clear();
    chosen=0;
}

void pad::win(){
    bool blackwin=true;
    bool whitewin=true;
    for(int i=0;i<10;i++)
        for(int j=0;j<5;j++){
            if(chess[i][j]==2)
                whitewin=false;
            if(chess[i][j]==1)
                blackwin=false;
        }
    if(turn==1){//黑方回合，若黑方无子或无法走动
        if(whitewin)
            emit here(1001);
        else if(allpa.size()==0)
            emit here(1001);
    }else{
        if(blackwin)
            emit here(1002);
        else if(allpa.size()==0)
            emit here(1002);
    }
}

posi::posi(int a,int b){
    this->l=a;          //直接设置位置
    this->x=b;
}

posi::posi(QPoint a){
    if(a.y()>2&&a.y()<=52){
        l=0;
    }else if(a.y()>52&&a.y()<=102){
        l=1;
    }else if(a.y()>102&&a.y()<=152){
        l=2;
    }else if(a.y()>152&&a.y()<=202){
        l=3;
    }else if(a.y()>202&&a.y()<=252){
        l=4;
    }else if(a.y()>252&&a.y()<=302){
        l=5;
    }else if(a.y()>302&&a.y()<=352){
        l=6;
    }else if(a.y()>352&&a.y()<=402){
        l=7;
    }else if(a.y()>402&&a.y()<=452){
        l=8;
    }else if(a.y()>452&&a.y()<=502){
        l=9;
    }
    x=5;
    if(l%2==0){
        if(a.x()>52&&a.x()<=102){                         //位置映射
            x=0;
        }else if(a.x()>152&&a.x()<=202){
            x=1;
        }else if(a.x()>252&&a.x()<=302){
            x=2;
        }else if(a.x()>352&&a.x()<=402){
            x=3;
        }else if(a.x()>452&&a.x()<=502){
            x=4;
        }
    }else{
        if(a.x()>2&&a.x()<=52){                         //位置映射
            x=0;
        }else if(a.x()>102&&a.x()<=152){
            x=1;
        }else if(a.x()>202&&a.x()<=252){
            x=2;
        }else if(a.x()>302&&a.x()<=352){
            x=3;
        }else if(a.x()>402&&a.x()<=452){
            x=4;
        }
    }
}

posi::posi(){
    l=10;x=5;
}

void posi::clear(){
    l=10;x=5;
}

path::path(){
    l.clear();
    x.clear();
    el.clear();
    ex.clear();
    n=0;
}

path::path(int _l,int _x,int _n){
    l.clear();
    x.clear();
    el.clear();
    ex.clear();
    l.append(_l);
    x.append(_x);
    n=_n;
}

void path::clear(){
    l.clear();
    x.clear();
    el.clear();
    ex.clear();
    n=0;
}
