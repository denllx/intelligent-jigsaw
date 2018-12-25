#include "setlevel.h"
#include "ui_setlevel.h"
#include "scene.h"
#include <QMovie>
#include <QDebug>

setlevel::setlevel(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::setlevel)
{
    father=static_cast<WelcomeScene*>(parent);
    ui->setupUi(this);
    //背景
    bg=new QMovie(":/images/images/setlevel.jpg");
    this->setScaledContents(true);
    bg->start();
    this->setMovie(bg);

    //点击确认按钮返回welcomescene，传回row和col数值
    connect(ui->ok,SIGNAL(clicked()),this,SLOT(confirmed()));

    //点击取消返回welcomescene
    connect(ui->reset,SIGNAL(clicked(bool)),this,SIGNAL(back()));

}

//确认难度，向父亲发送行列数
void setlevel::confirmed(){
    int row=ui->comboRow->currentText().toInt();
    int col=ui->comboCol->currentText().toInt();
    qDebug()<<"from setlevel:"<<row<<" "<<col;
    emit sendRowCol(row,col);
}

setlevel::~setlevel()
{
    delete bg;
    delete ui;
}
