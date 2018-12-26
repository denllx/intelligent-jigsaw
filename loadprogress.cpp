#include <QMessageBox>
#include <QDebug>
#include "loadprogress.h"
#include "ui_loadprogress.h"
#include "utils.h"

loadprogress::loadprogress(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::loadprogress)
{
    ui->setupUi(this);
    bg=new QMovie(":/images/images/setlevel.jpg");
    this->setMovie(bg);
    this->setScaledContents(true);
    bg->start();

    //在组合框中列出所有已有的进度
    QString dir=qApp->applicationDirPath();
    dir+="\\progress\\";
    QStringList filters;
    filters<<"*.json";
   QStringList alreadyexist=getdirnames(dir,filters);
   ui->inputname->addItems(alreadyexist);
    connect(ui->confirm,&QPushButton::clicked,[=](){
       QString text=ui->inputname->currentText();
       emit getfilename(text);
    });//点击确认，发送将加载的进度名
    connect(ui->cancel,&QPushButton::clicked,[=](){
        qDebug()<<"load progress:donothing";
        emit donothing();
    });
}

loadprogress::~loadprogress()
{
    delete ui;
}
