#include "initial.h"
#include "ui_initial.h"

initial::initial(QHostAddress a,QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint),
    ui(new Ui::initial)
{
    ui->setupUi(this);
    setWindowTitle("客户端");
    if(!a.isNull()){
        ip=a;
        ui->ip->setText(a.toString());
        ui->ip->setReadOnly(true);
        setWindowTitle("服务器");
    }
    ui->label_3->hide();
}

initial::~initial()
{
    delete ui;
}

void initial::on_o_clicked()
{
    if(ui->port->text()==""||ui->ip->text()=="")
        return;
    port=ui->port->text().toInt();
    if(!ui->ip->isReadOnly())//如果是客户端
        ip.setAddress(ui->ip->text());
    emit set(ip,port);
    ui->label_3->show();
    ui->o->hide();
    ui->c->hide();
}

void initial::on_c_clicked()
{
    emit closing();
    close();
}
