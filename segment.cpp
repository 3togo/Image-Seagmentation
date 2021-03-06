#include"segment.h"
#include"datastrcture.h"
#include<cmath>
#include<map>
#define THRESHOLD(c,size) (c/size)
static const int COLOR_NUM = 29, COLOR_NU_NO_GRAY = 24;
using namespace std;
bool operator<(const edge &a, const edge &b)  //重载运算符，用于边的比较
{
    return a.w < b.w;
}

static inline float diff(Mat &gusImg, int x1, int y1, int x2, int y2)
{
    CV_Assert(gusImg.type() == CV_32FC3);
    const Vec3f &p1 = gusImg.at<Vec3f>(y1, x1);
    const Vec3f &p2 = gusImg.at<Vec3f>(y2, x2);
    return sqrt(pow((p1[0] - p2[0]),2) + pow((p1[1] - p2[1]),2) + pow((p1[2] - p2[2]),2));
}
static Vec3b RandomColor()
{
    return Vec3b((uchar)(rand() % 200 + 25), (uchar)(rand() % 200 + 25), (uchar)(rand() % 200 + 25));
}
const Vec3b gColors[] =
{
    Vec3b(0, 0, 255), Vec3b(0, 255, 0), Vec3b(255, 0, 0), Vec3b(153, 0, 48), Vec3b(0, 183, 239),
    Vec3b(255, 255, 0), Vec3b(255, 126, 0), Vec3b(255, 194, 14), Vec3b(168, 230, 29),
    Vec3b(237, 28, 36), Vec3b(77, 109, 243), Vec3b(47, 54, 153), Vec3b(111, 49, 152), Vec3b(156, 90, 60),
    Vec3b(255, 163, 177), Vec3b(229, 170, 122), Vec3b(245, 228, 156), Vec3b(255, 249, 189), Vec3b(211, 249, 188),
    Vec3b(157, 187, 97), Vec3b(153, 217, 234), Vec3b(112, 154, 209), Vec3b(84, 109, 142), Vec3b(181, 165, 213),
    Vec3b(40, 40, 40), Vec3b(70, 70, 70), Vec3b(120, 120, 120), Vec3b(180, 180, 180), Vec3b(220, 220, 220)
};
region *SegGrah(edge *edges, int edgnum, int vecnum,float c)
{
    region* reg = new region(vecnum);
    std::sort(edges,edges+edgnum);
    float *threshold = new float[vecnum];
    for (int i = 0; i < vecnum; i++)
    {
        threshold[i] = THRESHOLD(c, 1);
    }
    for (int i = 0; i < edgnum; i++)
    {
        edge *pedge = &edges[i];
        int a = reg->getFlag(pedge->a);
        int b = reg->getFlag(pedge->b);
        if (a != b && (pedge->w <= threshold[a]) &&
            (pedge->w <= threshold[b]))
        {
            reg->joint(a, b);
            a = reg->getFlag(a);
            threshold[a]=pedge->w+THRESHOLD(c,reg->getSize(a));
        }

    }
    delete[] threshold;
    return reg;
}
int SegmentImage(Mat &_src3f, Mat &pImgInd, double ts, double tc, int tmin_size)
{
    CV_Assert(_src3f.type() == CV_32FC3);
    Mat gusImg;
    int imgWidth = _src3f.cols;
    int imgHeight = _src3f.rows;
    GaussianBlur(_src3f, gusImg, Size(), ts, 0, BORDER_REPLICATE);
    namedWindow("gusimage", WINDOW_AUTOSIZE);
    imshow("gusimage",gusImg);
    edge *edges = new edge[4*imgHeight*imgWidth];
    int num = 0;
    for (int j = 0; j < imgHeight; j++)     //构造8邻域
    {
        for (int i = 0; i < imgWidth; i++)
        {
            if (i < imgWidth - 1)
            {
                edges[num].a = j*imgWidth + i;
                edges[num].b = j*imgWidth + i + 1;
                edges[num].w = diff(gusImg,i,j,i+1,j);
                num++;
            }
            if (j < imgHeight - 1)
            {
                edges[num].a = j*imgWidth + i;
                edges[num].b = (j+1)*imgWidth + i;
                edges[num].w = diff(gusImg, i, j, i , j+1);
                num++;
            }
            if ((i < imgWidth - 1) && (j < imgHeight - 1))
            {
                edges[num].a = j*imgWidth + i;
                edges[num].b = (j+1)*imgWidth + i + 1;
                edges[num].w = diff(gusImg, i, j, i + 1, j+1);
                num++;

            }
            if ((i < imgWidth - 1) && j>0)
            {
                edges[num].a = j*imgWidth + i;
                edges[num].b = (j - 1)*imgWidth + i + 1;
                edges[num].w = diff(gusImg, i, j, i + 1, j - 1);
                num++;
            }
        }
    }
    region *reg = SegGrah(edges,num,imgHeight*imgWidth,tc);
    //去除小的块
    for (int i = 0; i < num; i++)
    {

        int a = reg->getFlag(edges[i].a);
        int b = reg->getFlag(edges[i].b);
        if ((a != b) && ((reg->getSize(a) < tmin_size) || (reg->getSize(b) < tmin_size)))
            reg->joint(a, b);
    }

    //给每一个区域块染色
    map <int, int> marker;
    //Mat regImg;
    pImgInd.create(gusImg.size(), CV_32S);
    int idxNum = 0;
    for (int j = 0; j < imgHeight; j++)
    {
        int *imgcol=pImgInd.ptr<int>(j);
        for (int  i = 0; i < imgWidth; i++)
        {
            int flag = reg->getFlag(j*imgWidth + i);
            if (marker.find(flag) == marker.end())
            {
                marker[flag] = idxNum++;
            }
            imgcol[i] = marker[flag];
        }

    }
    delete[] edges;
    delete reg;
    return idxNum;

}

// Show a label map. labelNum: how many number of random colors used for show, use default colors if is -1
//第三个参数为可选项
Mat Label(Mat& label1i, int labelNum, bool showIdx)
{
    bool useRandom = labelNum > 0;
    labelNum = useRandom ? labelNum : COLOR_NU_NO_GRAY;
    vector<Vec3b> colors(labelNum);
    if (useRandom)
        for (size_t i = 0; i < colors.size(); i++)
            colors[i] = RandomColor();
    else
        for (size_t i = 0; i < colors.size(); i++)
            colors[i] = gColors[i];
    Mat showImg = Mat::zeros(label1i.size(), CV_8UC3);
    for (int y = 0; y < label1i.rows; y++)  {
        Vec3b* showD = showImg.ptr<Vec3b>(y);
        const int* label = label1i.ptr<int>(y);
        for (int x = 0; x < label1i.cols; x++)
        if (label[x] >= 0){
            showD[x] = colors[label[x] % labelNum];
            if (showIdx)
                showD[x][2] = (unsigned char)(label[x]);
        }
    }

    return showImg;
}
