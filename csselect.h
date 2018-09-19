#ifndef CSSELECT_H
#define CSSELECT_H

#include <QDialog>

namespace Ui {
class csselect;
}

class csselect : public QDialog
{
    Q_OBJECT

public:
    explicit csselect(QWidget *parent = 0);
    ~csselect();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

signals:
    void se(int);
private:
    Ui::csselect *ui;
};

#endif // CSSELECT_H
