#include "bmpreader.h"
#include <QImage>
#include "stdlib.h"
#include "math.h"
#include <qDebug>

#define PI 3.14159
#define LENGTH_NAME_BMP 100//文件名的最大长度

using namespace std;

//变量定义
BITMAPFILEHEADER strHead;//文件头
RGBQUAD strPla[256];//256色调色板
BITMAPINFOHEADER strInfo;//图片信息头

//显示位图文件头信息
void showBmpHead(BITMAPFILEHEADER pBmpHead) {
    qDebug() << "位图文件头:";
    qDebug() << "文件大小:" << pBmpHead.bfSize ;
    qDebug() << "保留字_1:" << pBmpHead.bfReserved1 ;
    qDebug() << "保留字_2:" << pBmpHead.bfReserved2;
    qDebug() << "实际位图数据的偏移字节数:" << pBmpHead.bfOffBits;
}

void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead) {
    qDebug() << "位图信息头:" ;
    qDebug() << "结构体的长度:" << pBmpInforHead.biSize;
    qDebug() << "位图宽:" << pBmpInforHead.biWidth ;
    qDebug() << "位图高:" << pBmpInforHead.biHeight;
    qDebug() << "biPlanes平面数:" << pBmpInforHead.biPlanes;
    qDebug() << "biBitCount采用颜色位数:" << pBmpInforHead.biBitCount ;
    qDebug() << "压缩方式:" << pBmpInforHead.biCompression ;
    qDebug() << "biSizeImage实际位图数据占用的字节数:" << pBmpInforHead.biSizeImage;
    qDebug() << "X方向分辨率:" << pBmpInforHead.biXPelsPerMeter;
    qDebug() << "Y方向分辨率:" << pBmpInforHead.biYPelsPerMeter;
    qDebug() << "使用的颜色数:" << pBmpInforHead.biClrUsed;
    qDebug() << "重要颜色数:" << pBmpInforHead.biClrImportant;
}

//获取指定文件名的图像文件的像素矩阵
IMAGEDATA* getpixelarray(const char* filename,int& width,int& height) {
    IMAGEDATA *imagedata = NULL;//存储原图片的像素信息的二维数组
    FILE *fpi;
    fpi = fopen(filename, "rb");
    if (fpi != NULL) {
        WORD bfType;//文件类型
        fread(&bfType, 1, sizeof(WORD), fpi);
        if (0x4d42 != bfType)//不是bmp文件
        {
            qDebug() << "the file is not a bmp file!";
            return NULL;
        }
        fread(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpi);//读取文件头
        fread(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpi);//读取信息头

        //读取调色板
        for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++)
        {
            //存储的时候，一般去掉保留字rgbReserved
            fread((char *)&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpi);
            fread((char *)&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpi);
            fread((char *)&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpi);
        }

        width = strInfo.biWidth;
        height = strInfo.biHeight;
        imagedata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));//像素矩阵
        //imagedataRot = (IMAGEDATA*)malloc(2 * width * 2 * height * sizeof(IMAGEDATA));
        //初始化原始图片的像素数组
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                (*(imagedata + i * width + j)).blue = 0;//第i行第j列的蓝色分量初始化0
                (*(imagedata + i * width + j)).green = 0;
                (*(imagedata + i * width + j)).red = 0;
            }
        }
        fread(imagedata, sizeof(struct tagIMAGEDATA) * width, height, fpi);//将像素矩阵从文件读到imagedata
        fclose(fpi);
    }
    else
    {
        qDebug() << "cannot read bmp file in getpixelarray";
        return NULL;
    }
    return imagedata;
}

//根据像素矩阵保存图片，若成功返回true
bool savepixelarray(const char* filename,IMAGEDATA* imagedata,int width,int height) {
    //保存bmp图片
    FILE* fpw;
    if ((fpw = fopen(filename, "wb")) == NULL)
    {
        qDebug() << "cannot write bmp file in savepixelarray" ;
        return false;
    }
    WORD bfType_w = 0x4d42;
    fwrite(&bfType_w, 1, sizeof(WORD), fpw);//文件类型
    fwrite(&strHead, 1, sizeof(tagBITMAPFILEHEADER), fpw);//写入文件头
    strInfo.biWidth = width;
    strInfo.biHeight = height;
    fwrite(&strInfo, 1, sizeof(tagBITMAPINFOHEADER), fpw);//写入信息头
    //写入调色板数据
    for (unsigned int nCounti = 0; nCounti < strInfo.biClrUsed; nCounti++)
    {
        fwrite(&strPla[nCounti].rgbBlue, 1, sizeof(BYTE), fpw);
        fwrite(&strPla[nCounti].rgbGreen, 1, sizeof(BYTE), fpw);
        fwrite(&strPla[nCounti].rgbRed, 1, sizeof(BYTE), fpw);
    }
    //保存像素数据
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            fwrite(&(*(imagedata + i * width + j)).red, 1, sizeof(BYTE), fpw);//注意三条语句的顺序：否则颜色会发生变化
            fwrite(&(*(imagedata + i * width + j)).green, 1, sizeof(BYTE), fpw);
            fwrite(&(*(imagedata + i * width + j)).blue, 1, sizeof(BYTE), fpw);
        }
    }
    fclose(fpw);
    return true;
}

//由于bmp扫描顺序为从下到上，从左向右，需要将像素矩阵上下颠倒后进行显示
IMAGEDATA* revertImagedata(IMAGEDATA* imagedata,int width,int height) {
    IMAGEDATA* newdata;
    newdata = (IMAGEDATA*)malloc(width * height * sizeof(IMAGEDATA));//像素矩阵
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            *(newdata+(height-1-i)*width+j)= *(imagedata+i*width+j);
        }
    }
    return newdata;
}

/*
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    bmp_reader_gui w;
    IMAGEDATA* imagedata = getpixelarray();
    IMAGEDATA* newdata = revertImagedata(imagedata);
    //每行有多少个字节=每行有多少个像素*每个像素有多少个字节
    QImage image((const uchar*)newdata, width, height, width * 3, QImage::Format_RGB888);
    //获取像素数据
    const uchar* datafromqimage = image.constBits();
    savepixelarray(imagedata);
    QLabel* label = new QLabel(&w);
    label->setGeometry(0,0,image.width(), image.height());
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
    w.setMinimumWidth(image.width());
    w.setMinimumHeight(image.height());
    w.show();
    return a.exec();
}
*/
