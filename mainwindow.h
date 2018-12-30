#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QJsonObject>
#include <QLabel>
#include <QThread>
#include <vector>
#include "solver.h"
#include "scene.h"
#include "savepic.h"
#include "saveprogress.h"
#include "loadprogress.h"
#include "bmpreader.h"

class MusicPlayer;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();//初始化拼图界面
    void resume();//读档后继续
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

    void getRowCol(int,int);//获取来自scene的难度

    void savePicture();//保存图片

    void getfilename(QString filename);//获取保存图片的文件名并保存文件

    void savegame();//存档

    void loadgame();//读档

    void getprogressname(QString filename);//获取保存进度的名称并保存进度

    void getloadgame(QString filename);//获取读取进度的名称并加载进度

    void startBgm();//开始播放背景音乐

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
    bool isRunning;//游戏是否开始

    Scene* scene;//场景
    savepic* savedialog;//保存图片的对话框，用户需要输入文件名
    saveprogress* progressdialog;//进度保存的对话框，用户需要输入进度名
    loadprogress* loaddialog;//读档对话框，需要用户输入进度名
    MusicPlayer* music;//音乐播放器
    QThread musicThread;//音乐播放线程

    int pixelwidth,pixelheight;//一张大图每列/行像素数
    IMAGEDATA* originalData,*invertedData;//原始像素矩阵，上下颠倒后的像素矩阵

    void write(QJsonObject& json);
    void read(const QJsonObject& json);

};

#endif // MAINWINDOW_H
