#include <QMovie>
#include <QDebug>
#include <QMessageBox>
#include <QTextCodec>
#include "scene.h"
#include "mainwindow.h"
#include "ui_welcome.h"
#include "utils.h"

Scene::Scene(QWidget *parent) : QLabel(parent)
{
    father=static_cast<MainWindow*>(parent);
}

WelcomeScene::WelcomeScene(QWidget *parent):
    Scene(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
    this->setGeometry(0,0,850,700);
    this->setScaledContents(true);
    bg=new QMovie(":/images/images/welcome.jpg");
    bg->start();
    this->setMovie(bg);
    this->show();

    connect(ui->loadgame,&QPushButton::clicked,[=](){
        loaddialog=new loadprogress(this);
        loaddialog->setGeometry(300,200,loaddialog->width(),loaddialog->height());
        connect(loaddialog,&loadprogress::donothing,[=](){
            qDebug()<<"loaddialog:donothing";
            loaddialog->deleteLater();
        });
        connect(loaddialog,SIGNAL(getfilename(QString)),this,SLOT(getloadname(QString)));
        loaddialog->show();
    });
}

//获取加载进度名
//如果成功，将进度名传给父亲
void WelcomeScene::getloadname(QString filename){
    loaddialog->deleteLater();
    emit sendloadname(filename);
}

WelcomeScene::~WelcomeScene(){
    delete bg;
    delete ui;
}

//点击按钮设置难度
void WelcomeScene::on_btnSetlevel_clicked()
{
    widgetSetlevel=new setlevel(this);
    widgetSetlevel->setGeometry(200,200,widgetSetlevel->width(),widgetSetlevel->height());
    widgetSetlevel->show();
    connect(widgetSetlevel,SIGNAL(sendRowCol(int,int)),this,SLOT(getRowCol(int,int)));
    connect(widgetSetlevel,&setlevel::back,[=](){
        widgetSetlevel->deleteLater();
    });
}

//获取来自setlevel的行列数
void WelcomeScene::getRowCol(int row,int col){
    //删除setlevel窗口
    widgetSetlevel->deleteLater();
    //向父亲发送行列信息
    qDebug()<<"welcome scene:"<<row<<" "<<col;
    emit sendRowCol(row,col);
}


