#ifndef INITIAL_H
#define INITIAL_H

#include <QDialog>
#include <QtNetwork>

namespace Ui {
class initial;
}

class initial : public QDialog
{
    Q_OBJECT

public:
    explicit initial(QHostAddress,QWidget *parent = 0);
    ~initial();
    QHostAddress ip;
    int port;

private slots:
    void on_o_clicked();

    void on_c_clicked();

signals:
    void set(QHostAddress,int);
    void closing();
private:
    Ui::initial *ui;
};

#endif // INITIAL_H
