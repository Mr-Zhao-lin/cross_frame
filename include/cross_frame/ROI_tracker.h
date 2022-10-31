//
// Created by zdl on 22-9-26.
//

#ifndef ROI_TRACKER_ROI_TRACKER_H
#define ROI_TRACKER_ROI_TRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/tracking/tracking_legacy.hpp>
using namespace cv;
using namespace std;

void findSquares(const cv::Mat& image,vector<vector<Point> >& squares,cv::Mat &out);

template <class  Tracker_type>
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
    Tracker_type tracker;
    //cv::Ptr<cv::Tracker> tracker;
    //Ptr<legacy::Tracker>  tracker_legacy;
    bool track_status;
    Tracker_ROI(cv::Mat& _frame,const std::string &_trackerTypes="KCF");//初始化tracker,用鼠标框选
    Tracker_ROI(const cv::Mat &_frame,const std::string &_trackerTypes="KCF",const cv::Rect_<int> bbox={287, 23, 86, 320});//初始化tracker
    void New_frame(cv::Mat& _frame);
};


template <>
class Tracker_ROI<Ptr<legacy::Tracker>>   ///类的显世具体化，针对legacy中的tracker
{
    /*
     * 类方法：
     * 初始化跟踪器：tracker种类,初始frame，ROI选择
     * frame的输入
     * ROI的选择
     * 读取下一frame并追踪，返回bbox位置，size*/
public:
    cv::Rect_<double> bbox;
    cv::Mat frame;
    Ptr<legacy::Tracker> tracker;
    //cv::Ptr<cv::Tracker> tracker;
    //Ptr<legacy::Tracker>  tracker_legacy;
    bool track_status;
    Tracker_ROI(cv::Mat& _frame,const std::string &_trackerTypes="KCF");//初始化tracker,用鼠标框选
    Tracker_ROI(const cv::Mat &_frame,const std::string &_trackerTypes="KCF",const cv::Rect_<int> bbox={287, 23, 86, 320});//初始化tracker
    void New_frame(cv::Mat& _frame);
};


#endif //ROI_TRACKER_ROI_TRACKER_H


/////////////////////////////模板类声明和实现在同一文件
template <class  Tracker_type>
Tracker_ROI<Tracker_type>::Tracker_ROI(cv::Mat& _frame,const std::string &_trackerTypes) {
    frame=_frame;

    if (_trackerTypes == "MIL"){
        tracker = TrackerMIL::create();
    }
    else if(_trackerTypes == "KCF"){
        tracker = TrackerKCF::create();
    }
    else if (_trackerTypes == "GOTURN"){
        tracker = TrackerGOTURN::create();
    }
    else if (_trackerTypes == "CSRT"){
        tracker = TrackerCSRT::create();
    }
    else if (_trackerTypes == "MOSSE"){
        //tracker = TrackerCSRT::create();
       //legacy::TrackerMOSSE tracker_mosse;
        //tracker=tracker_mosse.create();
        //tracker=cv::legacy::TrackerMOSSE::create();
    }
    else{
        std::cout<<"ERROR:No such tracker";
    }
    bbox = selectROI(frame, true);
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    tracker->init(frame,bbox);

}
template <class  Tracker_type>
Tracker_ROI<Tracker_type>::Tracker_ROI(const cv::Mat &_frame,const std::string &_trackerTypes,const cv::Rect_<int> bbox):track_status(1) {
    frame=_frame;

    if (_trackerTypes == "MIL"){
        tracker = TrackerMIL::create();
    }
    else if(_trackerTypes == "KCF"){
        tracker = TrackerKCF::create();
    }
    else if (_trackerTypes == "GOTURN"){
        tracker = TrackerGOTURN::create();
    }
    else if (_trackerTypes == "CSRT"){
        tracker = TrackerCSRT::create();
    }
    else if (_trackerTypes == "MOSSE"){
        //tracker = TrackerCSRT::create();
       //legacy::TrackerMOSSE tracker_mosse;
        //tracker=tracker_mosse.create();
        //tracker=cv::legacy::TrackerMOSSE::create();
    }
    else{
        std::cout<<"ERROR:No such tracker";
    }
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    tracker->init(frame,bbox);

}
template <class  Tracker_type>
void Tracker_ROI<Tracker_type>::New_frame(cv::Mat &_frame) {
    frame=_frame;
    track_status = tracker->update(frame, bbox);
    if ( track_status)
    {
        // Tracking success : Draw the tracked object
        rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    }
    else
    {
        // Tracking failure detected.
        putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
    }
    imshow("Tracking", frame);
    int k = waitKey(1);
    if(k == 27)
    {
        std::exit(1);
    }
}














Tracker_ROI<Ptr<legacy::Tracker>>::Tracker_ROI(cv::Mat& _frame,const std::string &_trackerTypes) {
    frame=_frame;

    tracker=cv::legacy::TrackerMOSSE::create();

    bbox = selectROI(frame, true);
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    tracker->init(frame,bbox);

}

Tracker_ROI<Ptr<legacy::Tracker>>::Tracker_ROI(const cv::Mat &_frame,const std::string &_trackerTypes,const cv::Rect_<int> bbox):track_status(1) {
    frame=_frame;
    tracker=cv::legacy::TrackerMOSSE::create();
    rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    imshow("Tracking", frame);
    tracker->init(frame,bbox);

}



void Tracker_ROI<Ptr<legacy::Tracker>>::New_frame(cv::Mat &_frame) {
    frame=_frame;
    track_status = tracker->update(frame, bbox);
    if ( track_status)
    {
        // Tracking success : Draw the tracked object
        rectangle(frame, bbox, Scalar( 255, 0, 0 ), 2, 1 );
    }
    else
    {
        // Tracking failure detected.
        putText(frame, "Tracking failure detected", Point(100,80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,255),2);
    }
    imshow("Tracking", frame);
    int k = waitKey(1);
    if(k == 27)
    {
        std::exit(1);
    }
}