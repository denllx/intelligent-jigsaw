#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QMovie>
#include <QLabel>
#include "setlevel.h"
#include "loadprogress.h"
class MainWindow;

namespace Ui{
    class welcome;
}

class Scene : public QLabel
{
    Q_OBJECT
public:
    explicit Scene(QWidget *parent = nullptr);
    MainWindow* father;

signals:

public slots:
};

class WelcomeScene:public Scene{
    Q_OBJECT

public:
    explicit WelcomeScene(QWidget* parent=nullptr);
    ~WelcomeScene();

    int row,col;//通过setlevel获取，传递给父窗口mainwindow
private slots:
    void on_btnSetlevel_clicked();
    void getRowCol(int,int);//获取来自setlevel的行列数
    void getloadname(QString filename);//获取加载的进度名

signals:
    void sendRowCol(int,int);//向父亲发送行列信息
    void sendloadname(QString filename);//发送进度名

private:
    Ui::welcome* ui;
    QMovie* bg;//背景图片
    setlevel* widgetSetlevel;//选择难度
    loadprogress* loaddialog;//存档进度
};

#endif // SCENE_H
