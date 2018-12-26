#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QThread>
#include <QSound>
#include <QMediaPlayer>
#include <QDebug>
#include <mainwindow.h>

class MainWindow;

class MusicPlayer:public QObject{
    Q_OBJECT
public:
    explicit MusicPlayer(MainWindow* parent=nullptr):QObject(parent){}
    /*virtual void run() Q_DECL_OVERRIDE{
    }*/
    void bgm(){
         //播放背景音乐
        QString dir=qApp->applicationDirPath();
        dir+="/music/moon_light.mp3";
        QMediaPlayer* music=new QMediaPlayer;
        music->setVolume(50);
        music->setMedia(QUrl("qrc:/music/music/moon_light.mp3"));
        qDebug()<<dir;
        music->play();
    }
};

#endif // MUSICPLAYER_H
