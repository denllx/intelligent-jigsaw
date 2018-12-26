#include "mainwindow.h"
#include <QApplication>
#include <map>
#include <string.h>
#include <iostream>
#include <QMap>
#include <algorithm>
#include <iostream>
#include <QDebug>
#include <QSound>
#include <QMediaPlayer>
#include <QMediaPlaylist>
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    /*map<string,int> maps;
    maps["up"]=1;
    string c="up";
    //bool equal=(c=="up");
    //bool notfound=(maps.find(c)==maps.end());
    //cout<<equal<<endl;
    vector<string> ret{"up","down","left","right"};
    ret.erase(std::remove(
                  ret.begin(),
                  ret.end(),
                  "right"),
              ret.end());
    for (int i=0;i<ret.size();i++) cout<<ret[i]<<endl;
    return 0;*/
    //QSound::play(":/music/music/moon_light.wav");
    //QMediaPlayer  *music = new QMediaPlayer();//初始化音乐

    //QMediaPlaylist *playlist = new QMediaPlaylist();//初始化播放列表

    //playlist->setPlaybackMode(QMediaPlaylist::Loop);//设置播放模式(顺序播放，单曲循环，随机播放等)



    return 0;

}
