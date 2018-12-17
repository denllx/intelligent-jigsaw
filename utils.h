#ifndef UTILS_H
#define UTILS_H
/*
 * 一些常用且简单的功能函数
*/

void permute(int* a,int len);//对长度为len的数组进行乱置
void copy(int** dest, int** src, int m, int n) ;
bool same(int** a, int** b, int m, int n);


#endif // UTILS_H
