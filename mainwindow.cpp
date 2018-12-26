#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "scene.h"
#include "savepic.h"
#include "saveprogress.h"
#include "loadprogress.h"
#include "musicplayer.h"
#include <QFileDialog>
#include <QString>
#include <QSound>
#include <QMediaPlayer>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>
#include <QPainter>
#include <QTextCodec>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

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
    //QTimer::singleShot(10,this,SLOT(startBgm()));
    startBgm();
    scene=new WelcomeScene(this);
    scene->setGeometry(0,0,this->width(),this->height());
    connect(scene,SIGNAL(sendRowCol(int,int)),this,SLOT(getRowCol(int,int)));
    connect(scene,SIGNAL(sendloadname(QString)),this,SLOT(getloadgame(QString)));
    scene->show();
}

void MainWindow::startBgm(){
    music=new MusicPlayer();
    music->moveToThread(&musicThread);
    music->bgm();
    musicThread.start();
}

void MainWindow::init(){
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
    initidx=new int*[m];
    for (int i=0;i<m;i++) idx[i]=new int[n],initidx[i]=new int[n];

    //绘制格子，添加分割线
    for (int i=0;i<m*n;i++){
        grids[i]=new QLabel(this);
        grids[i]->setGeometry(0,0,gridwidth,gridheight);
        grids[i]->move(IMAGEX+gridwidth*(i%n),IMAGEY+gridheight*(i/n));
        grids[i]->setFrameShape(QFrame::Box);//添加分割线
        grids[i]->setPixmap((QPixmap(":/images/blank.jpg")));//添加空白背景
        idx[i/n][i%n]=i;
        grids[i]->show();
    }
    r=m-1;
    c=n-1;
    idx[r][c]=-1;//空白空格，以示区别

    //初始化数据
    steps=0;
    seconds=0;
    timer=new QTimer(this);
    displayTimer=new QTimer(this);
    isRunning=false;
    connect(timer,SIGNAL(timeout()),this,SLOT(tick()));//复位计时
    connect(displayTimer,SIGNAL(timeout()),this,SLOT(displayRet()));//显示解

    //设置界面
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    ui->reset->setEnabled(false);

    //初始化菜单栏
    ui->actionsave_picture->setEnabled(false);
    ui->save_game->setEnabled(false);

    //初始化求解器
    solver=nullptr;
}

//读档后继续
void MainWindow::resume(){
    gridwidth=GRIDSWIDTH/n;
    gridheight=GRIDSHEIGHT/m;

    //初始化大图
    originalImage=new QImage(filename);

    //初始化格子
    grids=new QLabel*[m*n];

    //初始化小图
    images=new QImage[m*n];

    //绘制格子，添加分割线
    for (int i=0;i<m*n;i++){
        grids[i]=new QLabel(this);
        grids[i]->setGeometry(0,0,gridwidth,gridheight);
        grids[i]->setFrameShape(QFrame::Box);//添加分割线
        grids[i]->show();
    }

    //为每个格子填充小图
     QImage tmp=originalImage->scaled(gridwidth*n,gridheight*m);
    for (int i=0;i<m*n;i++){
        images[i]=tmp.copy(gridwidth*(i%n),gridheight*(i/n),gridwidth,gridheight);
        grids[i]->setPixmap(QPixmap::fromImage(images[i]));
    }
    moveImage();

    //初始化数据
    timer=new QTimer(this);
    displayTimer=new QTimer(this);
    isRunning=false;
    connect(timer,SIGNAL(timeout()),this,SLOT(tick()));//复位计时
    connect(displayTimer,SIGNAL(timeout()),this,SLOT(displayRet()));//显示解

    //设置界面
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    ui->reset->setEnabled(false);

    //初始化菜单栏
    ui->actionsave_picture->setEnabled(false);
    ui->save_game->setEnabled(false);

    //初始化求解器
    solver=nullptr;
}

//设置难度
void MainWindow::getRowCol(int row,int col){
    m=row;
    n=col;
    scene->deleteLater();
    init();
}

//保存图片
void MainWindow::savePicture(){
    if (isRunning) timer->stop();
    savedialog=new savepic(this);
    savedialog->setGeometry(300,200,savedialog->width(),savedialog->height());
    savedialog->show();
    connect(savedialog,SIGNAL(savePicture(QString)),this,SLOT(getfilename(QString)));
    connect(savedialog,&savepic::donothing,this,[=](){
       savedialog->deleteLater();
       if (isRunning){
          timer->start();
       }
    });
}

//将图片存储为相应文件名
void MainWindow::getfilename(QString filename){
    savedialog->deleteLater();
    QImage resultimage=QImage(GRIDSWIDTH,GRIDSHEIGHT,QImage::Format_RGB32);
    QPainter* painter=new QPainter(&resultimage);
    int gridwidth=GRIDSWIDTH/n;//一个格子的宽高
    int gridheight=GRIDSHEIGHT/m;
    painter->setPen(Qt::black);
    for (int i=0;i<m*n;i++){//第i张小图
        int r=i/n;//在第几行
        int c=i%n;//在第几列
        int startx=gridwidth*c;
        int starty=gridheight*r;
        int index=idx[r][c];
        painter->drawRect(startx,starty,gridwidth,gridheight);//画黑框
        if (index==-1){
            painter->setBrush(Qt::white);
            painter->drawRect(startx,starty,gridwidth,gridheight);//画白色矩形
        }
        else painter->drawImage(startx,starty,images[index]);//画小图
    }
    QString dir=qApp->applicationDirPath();
    dir+="\\savedimages\\";
    QString basename=filename;
    QString outfname=dir+basename+".bmp";
    QStringList filters;
    filters<<"*.bmp";
    QStringList alreadyexist=getdirnames(dir,filters);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    for (auto name:alreadyexist){
        if (name==basename){
            QMessageBox::StandardButton ret=QMessageBox::information(this,
                                     QString::fromLocal8Bit("提醒"),
                                     QString::fromLocal8Bit("文件已经存在，是否替换？"),
                                     QMessageBox::Yes|QMessageBox::Cancel);
            if (ret==QMessageBox::Yes){
                break;
            }
            else{
                if (isRunning) timer->start();
                return;
            }
        }
    }
    bool ret=resultimage.save(outfname);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if (ret){
        QMessageBox::about(this,QString::fromLocal8Bit(""),
                           QString::fromLocal8Bit("保存成功!"));
    }
    else{
        QMessageBox::warning(this,QString::fromLocal8Bit("失败"),
                             QString::fromLocal8Bit("保存失败！"));
    }
    if (isRunning) timer->start();
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


void MainWindow::shuffle(){
    /*int shuffleTimes=10;//移动图片的次数
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
    }*/

    int* shuffledArray=new int[m*n];
    for (int i=0,len=m*n;i<len;i++) shuffledArray[i]=-1;
    permute(shuffledArray,m*n);//随机填入[0,1,...m*n-2]
    for (int i=0,len=m*n;i<len;i++){
        idx[i/n][i%n]=shuffledArray[i];
    }
    delete shuffledArray;

    qDebug()<<"after shuffle image,before move image："<<"\n";
    for (int i=0;i<m;i++){
        int* tmp=idx[i];
        QString s=QString("%1%2%3%4").arg(tmp[0]).arg(tmp[1]).arg(tmp[2]).arg(tmp[3]);
        qDebug()<<s;
    }
    qDebug()<<"\n";

    //将idx复制到initidx
    for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
            initidx[i][j]=idx[i][j];

    //将更新同步到ui
    moveImage();

    qDebug()<<"after moveimage："<<"\n";
    for (int i=0;i<m;i++){
        int* tmp=idx[i];
        QString s=QString("%1%2%3%4").arg(tmp[0]).arg(tmp[1]).arg(tmp[2]).arg(tmp[3]);
        qDebug()<<s;
    }
    qDebug()<<"\n";

    //判断可行性
    solver=new Solver(m,n,idx);
    isAble=solver->isAble();
    if (isAble){
        QMessageBox::about(this,"恭喜","恭喜你，这是一个有解的拼图！");
    }
    else{
        QMessageBox::about(this,"挺住","挺住，这个拼图没有解！");
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
                           IMAGEY+gridheight*(i/n));//更改qlabel的位置
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

    //能够保存图片
    ui->actionsave_picture->setEnabled(true);
    connect(ui->actionsave_picture,SIGNAL(triggered(bool)),this,SLOT(savePicture()));
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
        isRunning=false;
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
    musicThread.quit();
    musicThread.wait();
    delete ui;
    //因为originalImage,images,idx和本window都没有父子关系，所以都需要手动释放
    //if (originalImage) delete originalImage;
    //delete images;
    //for (int i=0;i<m;i++) delete idx[i];
    //delete idx;
    //delete grids;
}

//开始游戏
void MainWindow::on_start_clicked()
{
    if (originalImage==NULL) return;//无法开始
    if (timer->isActive()) timer->stop();
    //更新设置界面
    ui->seconds->setText(QString::number(seconds));
    ui->steps->setText(QString::number(steps));
    timer->start(1000);//开始计时
    isRunning=true;
    ui->start->setEnabled(false);
    ui->select->setEnabled(false);
    ui->reset->setEnabled(true);
    //能够存档
    ui->save_game->setEnabled(true);
    connect(ui->save_game,SIGNAL(triggered(bool)),this,SLOT(savegame()));
}

//重新开始
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
    //将idx的状态恢复为initidx并更新ui
    for (int i=0;i<m;i++)
        for (int j=0;j<n;j++)
            idx[i][j]=initidx[i][j];
    moveImage();
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
    if (isAble){
        if (!retPath.empty()) retPath.clear();
        //清空计时器和步数
        steps=0;
        seconds=0;
        timer->stop();
        isRunning=false;
        //求解正确路径
        retPath=solver->solve();
        if (retPath.empty()){
            QMessageBox::warning(this,"警告","这个拼图没有解哦！");
            return;
        }
        //每隔1s显示一帧解
        retId=0;
        retSteps=retPath.size();
        displayTimer->start(1000);
    }
    else{
       QMessageBox::warning(this,"警告","这个拼图没有解！");
    }

}

//显示解路径的下一帧
void MainWindow::displayRet(){
    if (retId>=retSteps) {
        displayTimer->stop();
        return;
    }
   idx=retPath[retId];
   moveImage();
   retId++;
   //显示步数和时间
   seconds++;
   steps++;
   ui->seconds->setText(QString::number(seconds));
   ui->steps->setText(QString::number(steps));
}

//获取读档文件名后读档
void MainWindow::getloadgame(QString filename){
    qDebug()<<"mainwindow::getloadgame"<<filename;
    QString dir=qApp->applicationDirPath();
    dir+="\\progress\\";
    QString absname=dir+filename+".json";
    scene->deleteLater();
    QFile loadFile(absname);
    if (!loadFile.open((QIODevice::ReadOnly))){
        qWarning("couldn't open file in mainwindow::loadgame!");
    }
    QByteArray saveData=loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    read(loadDoc.object());
    loadFile.close();
    resume();
}

//获取存档文件名后存档
void MainWindow::getprogressname(QString name){
    progressdialog->deleteLater();
    QString dirname=qApp->applicationDirPath();
    dirname+="\\progress\\";
    QString basename=name+".json";
    QStringList filters;
    filters<<"*.json";
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QStringList alreadyexist=getdirnames(dirname,filters);
    for (auto existname:alreadyexist){
        qDebug()<<"alreadyexist:"<<name;
        if (name==existname){
            QMessageBox::StandardButton rb=QMessageBox::information(this,
                                 QString::fromLocal8Bit("提示"),
                                 QString::fromLocal8Bit("该文件已经存在，是否替换?"),
                                     QMessageBox::Yes,
                                     QMessageBox::No);
            if (rb==QMessageBox::Yes){
                break;
            }
            else {
                timer->start();
                return;
            }
        }
    }
    QString filename=dirname+basename;
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)){
        qWarning("couldn't open file in mainwindow::savegame");
        return;
    }
    QJsonObject gameobject;
    write(gameobject);
    QJsonDocument savedoc(gameobject);
    saveFile.write(savedoc.toJson());
    saveFile.close();
    qDebug()<<"savegame success!";
    timer->start();
}

//读档
void MainWindow::loadgame(){
    if (isRunning) timer->stop();
}

void MainWindow::savegame(){
    timer->stop();
    progressdialog=new saveprogress(this);
    progressdialog->setGeometry(300,200,progressdialog->width(),progressdialog->height());
    progressdialog->show();
    connect(progressdialog,SIGNAL(progressname(QString)),this,SLOT(getprogressname(QString)));
    connect(progressdialog,&saveprogress::donothing,[=](){
       progressdialog->deleteLater();
       timer->start();
    });
}

void MainWindow::read(const QJsonObject& json){
    qDebug()<<"mainwindow::read";
    QJsonObject objectGameData=json["gameData"].toObject();
    m=objectGameData["m"].toInt();
    n=objectGameData["n"].toInt();
    r=objectGameData["r"].toInt();
    c=objectGameData["c"].toInt();
    steps=objectGameData["steps"].toInt();
    seconds=objectGameData["seconds"].toInt();
    isAble=objectGameData["isAble"].toBool();
    filename=objectGameData["filename"].toString();
    QJsonArray arrayidx=objectGameData["currentidx"].toArray();
    QJsonArray arrayinitidx=objectGameData["initidx"].toArray();
    //初始化编号
    idx=new int*[m];
    initidx=new int*[m];
    for (int i=0;i<m;i++) idx[i]=new int[n],initidx[i]=new int[n];
    for (int i=0,len=arrayidx.size();i<len;i++){
        idx[i/n][i%n]=arrayidx[i].toInt();
        qDebug()<<"loading idx:"<<idx[i/n][i%n];
        initidx[i/n][i%n]=arrayinitidx[i].toInt();
        qDebug()<<"loading init idx:"<<initidx[i/n][i%n];
    }
}

void MainWindow::write(QJsonObject& json){
    QJsonObject gamedataObject;
    gamedataObject["m"]=m;
    gamedataObject["n"]=n;
    gamedataObject["r"]=r;
    gamedataObject["c"]=c;
    gamedataObject["steps"]=steps;
    gamedataObject["seconds"]=seconds;
    gamedataObject["isAble"]=isAble;
    gamedataObject["filename"]=filename;
    QJsonArray arrayidx;
    for (int i=0;i<m*n;i++){
        arrayidx.append(idx[i/n][i%n]);
    }
    gamedataObject["currentidx"]=arrayidx;
    QJsonArray arrayinitidx;
    for (int i=0;i<m*n;i++){
        arrayinitidx.append(initidx[i/n][i%n]);
    }
    gamedataObject["initidx"]=arrayinitidx;
    json["gameData"]=gamedataObject;
}
