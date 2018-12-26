#ifndef LOADPROGRESS_H
#define LOADPROGRESS_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

namespace Ui {
class loadprogress;
}

class loadprogress : public QLabel
{
    Q_OBJECT

public:
    explicit loadprogress(QWidget *parent = 0);
    ~loadprogress();

signals:
    void getfilename(QString filename);
    void donothing();

private:
    Ui::loadprogress *ui;
    QMovie* bg;
};

#endif // LOADPROGRESS_H
