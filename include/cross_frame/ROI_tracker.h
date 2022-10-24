//
// Created by zdl on 22-9-26.
//

#ifndef ROI_TRACKER_ROI_TRACKER_H
#define ROI_TRACKER_ROI_TRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
using namespace cv;
using namespace std;

class Tracker_ROI
{
    /*
     * 类方法：
     * 初始化跟踪器：tracker种类,初始frame，ROI选择
     * frame的输入
     * ROI的选择
     * 读取下一frame并追踪，返回bbox位置，size*/
public:
    cv::Rect_<int> bbox;
    cv::Mat frame;
    cv::Ptr<cv::Tracker> tracker;
    Tracker_ROI(cv::Mat& _frame,const std::string &_trackerTypes="KCF");//初始化tracker,用鼠标框选
    Tracker_ROI(const cv::Mat &_frame,const std::string &_trackerTypes="KCF",const cv::Rect_<int> bbox={287, 23, 86, 320});//初始化tracker
    void New_frame(cv::Mat& _frame);
};

class Object_detector
{
    /*
    合作目标的检测与分离
    */

};


void select_color()
{

};

void findSquares(const cv::Mat& image,vector<vector<Point> >& squares,cv::Mat &out);

#endif //ROI_TRACKER_ROI_TRACKER_H
