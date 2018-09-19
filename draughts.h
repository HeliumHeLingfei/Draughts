#ifndef DRAUGHTS_H
#define DRAUGHTS_H

#include <QtWidgets>
#include <QtNetwork>
#include "pad.h"
#include "linking.h"
#include "linkingclient.h"
#include "csselect.h"
#include "initial.h"
#include <QSound>

namespace Ui {
class draughts;
}

class draughts : public QWidget
{
    Q_OBJECT

public:
    explicit draughts(QWidget *parent = 0);
    ~draughts();

private:
    Ui::draughts *ui;
    pad *cpad;
    csselect *cse;
    linking *link;
    linkingclient *clink;
    initial *ini;
    int cs;
    void gameover();

private slots:
    void setunable(int);
    void setenable(int);
    void getstart(int);
    void closing();
    void startclient(QHostAddress,int);
    void startserver(QHostAddress,int);
    void on_giveup_clicked();
    void on_draw_clicked();
    void win(int);
    void sound(int);
};

#endif // DRAUGHTS_H
