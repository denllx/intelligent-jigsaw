#include <QMessageBox>
#include <QTextCodec>
#include "savepic.h"
#include "ui_savepic.h"

savepic::savepic(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::savepic)
{
    ui->setupUi(this);

    bg=new QMovie(":/images/images/setlevel.jpg");//设置背景图片
    this->setMovie(bg);
    this->setScaledContents(true);
    bg->start();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    connect(ui->save,&QPushButton::clicked,[=](){//点击按钮保存图片
        QString filename=ui->picname->text();
        if (filename.isEmpty()){
            QMessageBox::warning(this,
                                 QString::fromLocal8Bit("提示"),
                                 QString::fromLocal8Bit("文件名不能为空！"));
        }
        else emit savePicture(filename);//将文件名告知父亲窗口
    });

    connect(ui->cancel,&QPushButton::clicked,[=](){
       emit donothing();
    });//取消保存
}

savepic::~savepic()
{
    delete bg;
    delete ui;
}
