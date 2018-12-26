#include "saveprogress.h"
#include <QMessageBox>
#include <QTextCodec>
#include "ui_saveprogress.h"

saveprogress::saveprogress(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::saveprogress)
{
    ui->setupUi(this);
    bg=new QMovie(":/images/images/setlevel.jpg");
    this->setMovie(bg);
    this->setScaledContents(true);
    bg->start();

    connect(ui->confirm,&QPushButton::clicked,[=](){
        QString filename=ui->inputname->text();
        if (filename.isEmpty()){
            QMessageBox::warning(this,
                                 QString::fromLocal8Bit("提示"),
                                 QString::fromLocal8Bit("进度名不能为空！"));
            return;
        }
        emit progressname(filename);
    });//点击确认按钮，传递进度名给父窗口
    connect(ui->cancel,&QPushButton::clicked,[=](){
        emit donothing();
    });//点击取消按钮，不保存
}

saveprogress::~saveprogress()
{
    delete ui;
}
