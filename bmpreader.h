#ifndef BMPREADER_H
#define BMPREADER_H

/*
 *  完成bmp文件相关数据结构的定义
    对bmp文件进行读取和写入操作
*/

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;


/*
 *bmp文件分成三部分：文件头，信息头，像素矩阵
 * 文件头包含文件类型
 * 信息头包含长宽（一列/行有多少个像素）
*/


typedef struct  tagBITMAPFILEHEADER {
    //WORD bfType;//文件类型，必须是0x424D，即字符“BM”
    DWORD bfSize;//文件大小
    WORD bfReserved1;//保留字
    WORD bfReserved2;//保留字
    DWORD bfOffBits;//从文件头到实际位图数据的偏移字节数
}BITMAPFILEHEADER;//文件头

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;//信息头大小
    LONG biWidth;//图像宽度
    LONG biHeight;//图像高度
    WORD biPlanes;//位平面数，必须为1
    WORD biBitCount;//每像素位数
    DWORD  biCompression; //压缩类型
    DWORD  biSizeImage; //压缩图像大小字节数
    LONG  biXPelsPerMeter; //水平分辨率
    LONG  biYPelsPerMeter; //垂直分辨率
    DWORD  biClrUsed; //位图实际用到的色彩数
    DWORD  biClrImportant; //本位图中重要的色彩数
}BITMAPINFOHEADER; //信息头

typedef struct tagRGBQUAD {
    BYTE rgbBlue; //该颜色的蓝色分量
    BYTE rgbGreen; //该颜色的绿色分量
    BYTE rgbRed; //该颜色的红色分量
    BYTE rgbReserved; //保留值
}RGBQUAD;//调色板

//像素信息
typedef struct tagIMAGEDATA
{
    BYTE red;
    BYTE green;
    BYTE blue;
}IMAGEDATA;

void showBmpHead(BITMAPFILEHEADER pBmpHead);//打印文件头
void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead);//打印信息头
IMAGEDATA* getpixelarray(const char* filename,int& width,int& height);//从指定的文件名获取像素矩阵及长宽
bool savepixelarray(const char* filename,
                    IMAGEDATA* imagedata,
                    int width,
                    int height);//将像素矩阵保存为指定文件名的bmp文件
IMAGEDATA* revertImagedata(IMAGEDATA* imagedata,int width,int height);//将像素矩阵上下颠倒用于显示
IMAGEDATA* shuffleImagedata(IMAGEDATA* imagedata,int** originalidx,int** idx,int m,int n,int width,int height);//将像素矩阵按照idx打乱


#endif // BMPREADER_H
