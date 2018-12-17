#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QLabel>
#include <vector>
#include "solver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);
    void splitImage();//切割图片
    void shuffle();//打乱图片们
    void moveImage();//根据idx移动小图
    void judge();//判断是否游戏结束

private slots:
    void on_select_clicked();

    void tick();//每过1s执行1次

    void displayRet();

    void on_start_clicked();

    void on_reset_clicked();

    void on_exit_clicked();

    void on_answer_clicked();

private:
    Ui::MainWindow *ui;
    QString filename;//图片文件名
    QImage* originalImage;//未经处理的大图
    int m,n;//图片被分为m行，n列
    int r,c;//空白格子所在的行、列
    int gridwidth,gridheight;//每个格子的宽高
    QImage* images;//小图
    QLabel** grids;//格子，用来显示image
    int steps;//记录步数
    int seconds;//记录用时
    int** idx;//idx[i][j]表示第i行第j列的小图的编号
    int** initidx;//存储idx在游戏开始前的状态，供重新开始用
    vector<int**> retPath;//解的路径
    bool isAble=false;//是否有解
    int retSteps;//解的步数
    int retId;//当前播放解的第几步
    QTimer* timer,*displayTimer;//全局时钟；仅用于显示解的时钟
    Solver* solver;//求解正确路径
};

#endif // MAINWINDOW_H
