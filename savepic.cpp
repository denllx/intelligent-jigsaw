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

    connect(ui->save,&QPushButton::clicked,[=](){//点击按钮保存图片
        QString filename=ui->picname->text();
        emit savePicture(filename);//将文件名告知父亲窗口
    });
}

savepic::~savepic()
{
    delete bg;
    delete ui;
}
