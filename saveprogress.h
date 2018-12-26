#ifndef SAVEPROGRESS_H
#define SAVEPROGRESS_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

namespace Ui {
class saveprogress;
}

class saveprogress : public QLabel
{
    Q_OBJECT

public:
    explicit saveprogress(QWidget *parent = 0);
    ~saveprogress();

signals:
    void progressname(QString filename);
    void donothing();

private:
    Ui::saveprogress *ui;
    QMovie* bg;
};

#endif // SAVEPROGRESS_H
