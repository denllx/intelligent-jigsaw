#include <QtMath>
#include "utils.h"

//对长度为len，值为-1的数组a随机填入[0,1,....len-2]
//最后一个没有填的元素保持-1，对应空白
void permute(int* a,int len){
    for (int num=0;num<len-1;num++){
        //随机挑选一个位置将num放入
        int j;
        while(1){
            j=qrand()%len;//插入的下标为[0,len-1]
            if (a[j]==-1)
                break;
        }
        a[j]=num;
    }
}

void copy(int** dest, int** src, int m, int n) {
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            dest[i][j] = src[i][j];
}


bool same(int** a, int** b, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j] != b[i][j])
                return false;
        }
    }
    return true;
}
