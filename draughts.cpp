#include "draughts.h"
#include "ui_draughts.h"

draughts::draughts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::draughts)
{
    ui->setupUi(this);
    cpad= new pad(ui->frame);
    ui->frame->setFrameStyle(QFrame::Panel|QFrame::Raised);             //设置画布位置
    ui->frame->setLineWidth(4);
    cpad->move(4,4);
    cpad->show();

    cse=new csselect();
    connect(cse,SIGNAL(se(int)),this,SLOT(getstart(int)));
    connect(cpad,SIGNAL(here(int)),this,SLOT(setunable(int)));
    connect(cpad,SIGNAL(here(int)),this,SLOT(win(int)));
    connect(cpad,SIGNAL(sound(int)),this,SLOT(sound(int)));
    cse->exec();
}

draughts::~draughts()
{
    delete ui;
}

void draughts::getstart(int a){
    cs=a;
    QHostAddress address;
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress ad,info.addresses())
    {
         if(ad.protocol() == QAbstractSocket::IPv4Protocol) {
             address.setAddress(ad.toString());
             qDebug()<<ad.toString();
             break;
         }
    }
    if(cs==1){//开启服务器
        ini=new initial(address);
        connect(ini,SIGNAL(closing()),this,SLOT(closing()));
        connect(ini,SIGNAL(set(QHostAddress,int)),this,SLOT(startserver(QHostAddress,int)));
        ini->exec();
        setWindowTitle("服务器");
    }else{//开启客户端
        ini=new initial(QHostAddress(QHostAddress::Null));
        connect(ini,SIGNAL(closing()),this,SLOT(closing()));
        connect(ini,SIGNAL(set(QHostAddress,int)),this,SLOT(startclient(QHostAddress,int)));
        ini->exec();
        setWindowTitle("客户端");
    }
}

void draughts::startserver(QHostAddress ip, int port){

    link=new linking(port);
    connect(link,SIGNAL(sc()),ini,SLOT(close()));
    connect(cpad,SIGNAL(here(int)),link,SLOT(sendmes(int)));

    connect(link,SIGNAL(set(int)),cpad,SLOT(there(int)));
    connect(link,SIGNAL(set(int)),this,SLOT(setenable(int)));
    connect(link,SIGNAL(set(int)),this,SLOT(win(int)));
    ui->label->setText("白方回合");
    ui->frame->setEnabled(true);

}

void draughts::startclient(QHostAddress ip, int port){

    clink=new linkingclient(ip,port);
    connect(clink,SIGNAL(sc()),ini,SLOT(close()));
    connect(cpad,SIGNAL(here(int)),clink,SLOT(sendmes(int)));

    connect(clink,SIGNAL(set(int)),cpad,SLOT(there(int)));
    connect(clink,SIGNAL(set(int)),this,SLOT(setenable(int)));
    connect(clink,SIGNAL(set(int)),this,SLOT(win(int)));
    ui->frame->setEnabled(false);
    ui->label->setText("白方回合");
}

void draughts::setunable(int a){
    if(a>=1000)
        return;
    if(cs==2)
        ui->label->setText("白方回合");
    else
        ui->label->setText("黑方回合");
    ui->frame->setEnabled(false);
}

void draughts::setenable(int a){
    if(a>=1000)
        return;
    if(cs==1)
        ui->label->setText("白方回合");
    else
        ui->label->setText("黑方回合");
    ui->frame->setEnabled(true);
}

void draughts::closing(){
    exit(0);
}

void draughts::on_giveup_clicked()
{
    if(cs==1){
        link->sendmes(1001);
        win(1001);
    }
    else{
        clink->sendmes(1002);
        win(1002);
    }
}

void draughts::on_draw_clicked()
{
    if(cs==1)
        link->sendmes(1003);
    else
        clink->sendmes(1003);
}

void draughts::gameover(){
    ui->frame->setEnabled(false);
    ui->draw->setEnabled(false);
    ui->giveup->setEnabled(false);
}

void draughts::win(int a){
    if(a<1000||!ui->draw->isEnabled())
        return;
    if(a==1001){
        if(cs==1)
            sound(3);
        else
            sound(1);
        ui->label->setText("白方回合");
        gameover();
        int r=QMessageBox::information(this, tr("游戏结束"),"黑方获胜",QMessageBox::Ok);
    }
    if(a==1002){
        if(cs==2)
            sound(3);
        else
            sound(1);
        ui->label->setText("黑方回合");
        gameover();
        int r=QMessageBox::information(this, tr("游戏结束"),"白方获胜",QMessageBox::Ok);
    }
    if(a==1000){
        sound(2);
        gameover();
        int r=QMessageBox::information(this, tr("游戏结束"),"和棋",QMessageBox::Ok);
    }
    if(a==1003){
        int ret = QMessageBox::warning(this, tr("求和"), tr("是否同意对手的求和?"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            if(cs==1)
                link->sendmes(1000);
            else
                clink->sendmes(1000);
            sound(2);
            gameover();
            int r=QMessageBox::information(this, tr("游戏结束"),"和棋",QMessageBox::Ok);
        } else {
            if(cs==1)
                link->sendmes(1004);
            else
                clink->sendmes(1004);
        }
    }
    if(a==1004)
        int r=QMessageBox::information(this, tr("求和"),"对方拒绝你的求和",QMessageBox::Ok);
}

void draughts::sound(int a){
    if(a==1)
        QSound::play(":/sound/305.wav");
    if(a==2)
        QSound::play(":/sound/draw.wav");
    if(a==3)
        QSound::play(":/sound/lose.wav");
    if(a==4)
        QSound::play(":/sound/go.wav");
}
