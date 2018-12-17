#include "mainwindow.h"
#include <QApplication>
#include <map>
#include <string.h>
#include <iostream>
#include <QMap>
#include <algorithm>
#include <iostream>
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

}
