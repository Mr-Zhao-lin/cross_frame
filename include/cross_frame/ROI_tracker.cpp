//
// Created by zdl on 22-9-26.
//
#include "ROI_tracker.h"
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>

using namespace cv;
using namespace std;



static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2) / sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
//############################################两个函数用于对点集进行逆时针排序
//向顺时针排序，若a在b的逆时针方向（即a在b前面），则返回true
bool PointCmp(Point a, Point b, Point center)
{
    double angle_a=atan2(-(a.y-center.y),a.x-center.x);
    double angle_b=atan2(-(b.y-center.y),b.x-center.x);
    if(angle_a>angle_b)
    {
        return true;
    }
    else
    {
        return false;
    }
    
} 

void ClockwiseSortPoints(vector<Point> &vPoints)
{//注意，以输入的顺序第一个点为排序的基准点
    //计算重心
    Point center;
    double X = 0, Y = 0;
    for (int i = 0; i < vPoints.size(); i++)
    {
        X += vPoints[i].x;
        Y += vPoints[i].y;
    }
    center.x = (int)X / vPoints.size();
    center.y = (int)Y / vPoints.size();
    //冒泡排序
    for (int i = 0; i < vPoints.size() ; i++)
    {
        for (int j = 0; j < vPoints.size() - i-1 ; j++)
        {
            if (PointCmp(vPoints[j], vPoints[j + 1], center))
            {
                //此时按照顺时针排序
            }
            else
            {
                Point tmp = vPoints[j];
                vPoints[j] = vPoints[j + 1];
                vPoints[j + 1] = tmp;
            }
        }
    }
}

void findSquares(const cv::Mat& image,vector<vector<Point> >& squares,cv::Mat &out)
{
  
    
	int thresh = 200;//边缘检测的梯度阈值
	//vector<vector<Point> > squares;
	squares.clear();
	Mat src,dst, gray;
    gray=image.clone();
	//滤波增强边缘检测
	medianBlur(gray, gray, 9);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
    //检测边缘
    Canny(gray, gray, 50, 200, 3);//第五个参数是Sober算子大小
    //闭运算
    dilate(gray, gray, Mat(), Point(-1, -1));
    erode(gray, gray, Mat(), Point(-1, -1));
    

    //imshow("预处理完成", gray);


    // 轮廓查找
    findContours(gray, contours, hierarchy, cv::RetrievalModes::RETR_LIST, CHAIN_APPROX_SIMPLE);

    
    //cout<<"共找到"<<contours.size()<<"条轮廓"<<endl;
    // 检测所找到的轮廓
    //##########################################将其中最长的几条轮廓提出来##########################################
    vector<vector<Point> > contours_longest;
    
    if (contours.size()>2)
    {
        contours_longest.push_back(contours[0]);
        contours_longest.push_back(contours[1]);
            for (size_t i = 0; i < contours.size(); i++)
        {
            if (contours[i].size()>contours_longest[1].size())
            {
                if(contours[i].size()>contours_longest[0].size())
                {
                    contours_longest[0]=contours[i];
                }
                else
                {
                    contours_longest[1]=contours[i];
                }        
            }
        }
    }
    else if(contours.size()==0)
    {
        return;
    }
    else 
    {
        contours_longest=contours;
    }

    //##########################################对这几条长的轮廓进行拟合##########################################
    vector<Point> approx;
    for(size_t i = 0; i < contours_longest.size(); i++)
    {
        approx.clear();
        //cout<<"最长的第"<<i<<"条轮廓长度为"<<contours_longest[i].size()<<endl;
        bool flag=0;
        for(size_t j=0;j<contours_longest[i].size();j++)
        {
            if((fabs(contours_longest[i][j].x-image.cols/2)>(image.cols/2-20))||(fabs(contours_longest[i][j].y-image.rows/2)>(image.rows/2-20)))
            {
                //把边界上的去掉
                //cout<<"该轮廓在边界附近"<<endl;
                flag=1;
                continue;
            }
        }
        if(flag==1)
        {
            continue;
        }
        //使用图像轮廓点进行多边形拟合
        approxPolyDP(Mat( contours_longest[i]), approx, arcLength(Mat( contours_longest[i]), true)*0.02,false);
        //cout<<"拟合得到的多边形顶点有"<<approx.size()<<endl;
        if(approx.size()<4)
        {
            //cout<<"approx.size()<4"<<endl;
            continue;
        }
       //##########################################对每个拟合得到的多边形聚类为四边形##########################################
        Mat approx_data=Mat(approx);//.reshape(1);
        approx_data.convertTo(approx_data,CV_32F);
        Mat labels_kmeans;
        TermCriteria criteria(TermCriteria::EPS+TermCriteria::COUNT,5,3);//终止标准类；参数：终止方法，最大迭代此书，精度
        Mat centers;
        kmeans(approx_data,4,labels_kmeans,criteria,4,KMEANS_PP_CENTERS,centers);//kmeans聚类
        approx=Mat_<Point> (centers.reshape(2));
        ClockwiseSortPoints(approx);
        //##########################################对这些四边形进行判别##########################################
        
        if (fabs(contourArea(Mat(approx))) > 30000 )
        {
            double maxCosine = 0;
            for (int j = 2; j < 5; j++)
            {
                // 求轮廓边缘之间角度的最大余弦
                double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                maxCosine = MAX(maxCosine, cosine);
            }

            if (maxCosine < 0.4)
            {
                squares.push_back(approx);
            }
            else
            {
                //cout<<"maxCosine > 1"<<endl;
            }
        }
        else
        {
            //cout<<"该多边形面积过小"<<endl;
            //cout<<"四个点为："<<Mat(approx)<<"                    面积为"<<fabs(contourArea(Mat(approx)))<<endl;
        }
       
	
    }
    
	//画出矩形
    //cout<<"最终得到的矩形个数"<<squares.size()<<endl;

	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];
        
		int n = (int)squares[i].size();
        polylines(out, &p, &n, 1, true, Scalar(0, 0, 255), 4);
	}
	imshow("out",out);
}

