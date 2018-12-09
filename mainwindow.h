#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>
#include <QLabel>

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

    void on_start_clicked();

    void on_reset_clicked();

    void on_exit_clicked();

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
    QTimer* timer;

};

#endif // MAINWINDOW_H
