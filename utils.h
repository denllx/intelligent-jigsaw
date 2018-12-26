#ifndef UTILS_H
#define UTILS_H
/*
 * 一些常用且简单的功能函数
*/
#include <QStringList>
#include <QDir>


void permute(int* a,int len);//对长度为len的数组进行乱置
void copy(int** dest, int** src, int m, int n) ;
bool same(int** a, int** b, int m, int n);
QStringList getdirnames(const QString& path,const QStringList& filter);//获取一个文件夹下所有满足后缀的文件


#endif // UTILS_H
