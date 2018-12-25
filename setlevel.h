#ifndef SETLEVEL_H
#define SETLEVEL_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class setlevel;
}

class WelcomeScene;

class setlevel : public QLabel
{
    Q_OBJECT

public:
    explicit setlevel(QWidget *parent = 0);
    ~setlevel();

public slots:
    void confirmed();//确认难度

signals:
    void sendRowCol(int,int);//向父亲发送行列数
    void back();

private:
    Ui::setlevel *ui;
    QMovie* bg;//背景图片
    int level,row;
    WelcomeScene* father;
};

#endif // SETLEVEL_H
