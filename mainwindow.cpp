#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>

const int GRIDSWIDTH=540;
const int GRIDSHEIGHT=540;//格子的总宽高
const int IMAGEX=20;
const int IMAGEY=80;//图片的左上角坐标

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(850,700);

    //TODO输入m,n
    m=3;
    n=4;
    gridwidth=GRIDSWIDTH/n;
    gridheight=GRIDSHEIGHT/m;

    //初始化大图
    originalImage=NULL;

    //初始化格子
    grids=new QLabel*[m*n];

    //初始化小图
    images=new QImage[m*n];

    //初始化编号
    idx=new int*[m];
    for (int i=0;i<m;i++) idx[i]=new int[n];

    //绘制格子，添加分割线
    for (int i=0;i<m*n;i++){
        grids[i]=new QLabel(this);
        grids[i]->setGeometry(0,0,gridwidth,gridheight);
        grids[i]->move(IMAGEX+gridwidth*(i%n),IMAGEY+gridheight*(i/n));
        grids[i]->setFrameShape(QFrame::Box);//添加分割线
        grids[i]->setPixmap((QPixmap(":/images/blank.jpg")));//添加空白背景
        idx[i/n][i%n]=i;
    }
    r=m-1;
    c=n-1;
    idx[r][c]=-1;//空白空格，以示区别

    //初始化数据
    steps=0;
    seconds=0;
    timer=new QTimer(this);
    displayTimer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(tick()));//复位计时
    connect(displayTimer,SIGNAL(timeout()),this,SLOT(displayRet()));//显示解

    //设置界面
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    ui->reset->setEnabled(false);
}

//将大图分为小图，按照顺序放进格子，最后一个为空白
void MainWindow::splitImage(){
    QImage tmp=originalImage->scaled(gridwidth*n,gridheight*m);
    for (int i=0;i<m*n;i++){
        //从大图中截取子图
        images[i]=tmp.copy(gridwidth*(i%n),gridheight*(i/n),gridwidth,gridheight);
        grids[i]->setPixmap(QPixmap::fromImage(images[i]));
        idx[i/n][i%n]=i;
    }
    r=m-1;
    c=n-1;
    idx[r][c]=-1;
    QPixmap blank(":/images/blank.jpg");
    grids[m*n-1]->setPixmap(blank);
}

//打乱图片
void MainWindow::shuffle(){
    int shuffleTimes=10;//移动图片的次数
    for (int i=0;i<shuffleTimes;++i){
        int row=0,col=0;
        for (int j=0;j<m*n;j++){
            if (idx[j/n][j%n]==-1){
                row=j/n;
                col=j%n;
                break;
            }
        }//找到空白图片所在格子的位置
        int direction=qrand()%4;//4个方向
        switch (direction) {
        case 0://空格向下移动
            if (row<m-1){
                idx[row][col]=idx[row+1][col];
                idx[row+1][col]=-1;
            }
            break;
        case 1://上
            if (row>0){
                idx[row][col]=idx[row-1][col];
                idx[row-1][col]=-1;
            }
            break;
        case 2://左
            if (col>0){
                idx[row][col]=idx[row][col-1];
                idx[row][col-1]=-1;
            }
            break;
        case 3://右
            if (col<n-1){
                idx[row][col]=idx[row][col+1];
                idx[row][col+1]=-1;
            }
            break;
        default:
            break;
        }
    }
    //将更新同步到ui
    moveImage();
    //判断可行性
    if (able){
        QMessageBox::about(this,"恭喜","恭喜你，这是一个有解的拼图！");
    }
    else{
        QMessageBox::about(this,"挺住","挺住，这个拼图没有解！");
        //TODO：更换拼图
    }
}

//根据idx矩阵移动格子
void MainWindow::moveImage(){
    for (int i=0;i<m*n;i++){
        int index=idx[i/n][i%n];
        if (index==-1) {
            index=m*n-1;
            r=i/n;
            c=i%n;//更新空白格所在的位置
        }
        grids[index]->move(IMAGEX+gridwidth*(i%n),
                           IMAGEY+gridheight*(i/n));
    }
}


//选择图片
void MainWindow::on_select_clicked()
{
    QString selectedName=QFileDialog::getOpenFileName(this,
                                                      tr("选择一张图片"),
                                                      "",
                                                      "Images(*.png *.jpg)");
    if (selectedName.isEmpty()){
        return;
    }

    filename=selectedName;
    if (originalImage!=NULL){
        delete originalImage;
        originalImage=NULL;
    }
    originalImage=new QImage(filename);//更换大图
    //在旁边显示还原后的图像，作为提示
    QImage goalImage=originalImage->scaled(ui->goal->width(),ui->goal->height());
    ui->goal->setPixmap(QPixmap::fromImage(goalImage));
    splitImage();
    shuffle();//打乱图片
}

//用户点击WASD分别让空格向上、向左、向下、向右移动
void MainWindow::keyPressEvent(QKeyEvent *event){
    if (!timer->isActive()) {
        return;
    }
    bool valid=false;//是否为有效移动
    switch (event->key()) {
    case Qt::Key_W:
        qDebug()<<"W";//上
        if (r>0){
            idx[r][c]=idx[r-1][c];
            idx[r-1][c]=-1;
            valid=true;
        }
        break;
    case Qt::Key_A://左
        qDebug()<<"A";
        if (c>0){
            idx[r][c]=idx[r][c-1];
            idx[r][c-1]=-1;
            valid=true;
        }
        break;
    case Qt::Key_S://下
        qDebug()<<"S";
        if (r<m-1){
            idx[r][c]=idx[r+1][c];
            idx[r+1][c]=-1;
            valid=true;
        }
        break;
    case Qt::Key_D://右
        qDebug()<<"D";
        if (c<n-1){
            idx[r][c]=idx[r][c+1];
            idx[r][c+1]=-1;
            valid=true;
        }
        break;
    default:
        break;
    }
    if (valid) {
        moveImage();
        steps++;
        ui->steps->setText(QString::number(steps));
    }
    judge();
}

//若成功复原，出现提示框
void MainWindow::judge(){
    bool finish=true;
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            if (i!=m-1||j!=n-1){//非空白格子
                if (idx[i][j]!=i*n+j){
                    finish=false;
                    break;
                }
            }
            else{
                if (idx[i][j]!=-1){
                    finish=false;
                    break;
                }
            }
        }
        if (!finish) break;
    }
    if (finish){
        grids[m*n-1]->setPixmap(QPixmap::fromImage(images[m*n-1]));//将图复原
        timer->stop();
        ui->select->setEnabled(true);
        ui->start->setEnabled(true);
        ui->reset->setEnabled(false);
        QMessageBox::about(this,"成功","恭喜你成功复原！");
    }

}

void MainWindow::tick(){
    seconds++;
    ui->seconds->setText(QString::number(seconds));
}

MainWindow::~MainWindow()
{
    delete ui;
    //因为originalImage,images,idx和本window都没有父子关系，所以都需要手动释放
    if (originalImage) delete originalImage;
    delete images;
    for (int i=0;i<m;i++) delete idx[i];
    delete idx;
    delete grids;
}

//开始游戏
void MainWindow::on_start_clicked()
{
    if (originalImage==NULL) return;//无法开始
    if (timer->isActive()) timer->stop();
    //初始化数据
    seconds=0;
    steps=0;
    //更新设置界面
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    shuffle();
    timer->start(1000);//开始计时
    ui->start->setEnabled(false);
    ui->select->setEnabled(false);
    ui->reset->setEnabled(true);
}

//重新来过
void MainWindow::on_reset_clicked()
{
    if (originalImage==NULL) return;
    seconds=0;
    steps=0;
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    if (timer->isActive()){
        timer->stop();
    }
    grids[m*n-1]->setPixmap(QPixmap(":/images/blank.jpg"));
    shuffle();
    timer->start(1000);
}

//退出游戏
void MainWindow::on_exit_clicked()
{
    close();
}

//显示从当前状态到目标的移动路径
void MainWindow::on_answer_clicked()
{
    //每隔1s显示一帧解
    retId=0;
    retSteps=ret.size();
    displayTimer->start(1000);
}

//显示解路径的下一帧
void MainWindow::displayRet(){
    if (retId>=retSteps) {
        displayTimer->stop();
        return;
    }
   idx=ret[retId];
   moveImage();
   retId++;
}
