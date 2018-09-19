#include "csselect.h"
#include "ui_csselect.h"

csselect::csselect(QWidget *parent) :
    QDialog(parent,Qt::WindowTitleHint),
    ui(new Ui::csselect)
{
    ui->setupUi(this);
    setWindowTitle("Draughts");
}

csselect::~csselect()
{
    delete ui;
}

void csselect::on_pushButton_clicked()
{
    emit se(1);
    close();
}

void csselect::on_pushButton_2_clicked()
{
    emit se(2);
    close();
}
