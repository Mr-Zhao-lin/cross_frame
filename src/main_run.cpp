//
// Created by zdl on 22-9-26.
//
#include<ros/ros.h>  
#include "ROI_tracker.h"
#include "opencv2/opencv.hpp"
#include "opencv2/tracking.hpp"
#include <opencv2/core/ocl.hpp>
#include"time.h"
using namespace cv;
using namespace std;
#define i_type 4             //设置跟踪器类型
    
#if(i_type>=4)
typedef Ptr<legacy::Tracker> tracker_type; //legacy tracker
#else
typedef Ptr<cv::Tracker>  tracker_type;
#endif
void dectect_square(VideoCapture &video,vector<vector<Point> > &squares);
void track_square(VideoCapture &video,vector<vector<Point> > &squares);

int frame_num=0;
int main(int argc, char **argv)
{
   
   
    //Tracker_ROI track_roi(frame,trackerType);//初始化

    VideoCapture capture;
	capture.open("/home/zdl/catkin_ws/src/cross_frame/src/2022-10-31-005320.webm");
	if (!capture.isOpened()) {
		printf("could not find video");
		return -1;
	}
    //测试时候注释调****************************************************8
    bool if_dectect=1;//跟踪(0)还是检测(1)
    vector<vector<Point> > squares;
	Mat frame,mask,frame_hsv,squares_picture;
    static  int fps = 0;
    clock_t start,finish;
    start = clock();//初始化

    int i=0;
    while(1)
    {
        // 检测框
        /*while (capture.read(frame))
        {
            imwrite("/home/zdl/catkin_ws/src/cross_frame/src/img/"+to_string(i)+".png",frame);
            i++;
        }
        */
        dectect_square(capture,squares);
        
        //  跟踪框    
       track_square(capture,squares);
       finish = clock();//初始化结束时间
        fps = frame_num/((double)(finish - start) / CLOCKS_PER_SEC);
        frame_num = 0;
        start= finish;
        cout<<"频率"<<fps<<endl;
        
    }

    return 0;
}
// betaflight  mpu6050
//
void dectect_square(VideoCapture &video,vector<vector<Point> > &squares)
{
    cout<<"dectecting squares"<<endl;
    squares.clear();
    Mat frame,mask,frame_hsv,squares_picture;
    while (video.read(frame)) 
    {   
        frame_num++;
        imshow("input_video", frame);
        cv::cvtColor(frame, frame_hsv,cv::COLOR_BGR2HSV);
        inRange(frame_hsv,Scalar((324.0/360.0)*180,0.470*255,0.180*255),Scalar((350.0/360.0)*180,0.78*255,0.625*255),mask);//过滤
		//inRange(frame_hsv,Scalar(0.46*180,0.36*255,0.0*255),Scalar(0.6*180,0.7*255,1.0*255),mask);//过滤
        imshow("frame_hsv",mask);
       squares_picture=Mat::ones(mask.size(),mask.type())*255;
        findSquares( mask, squares,squares_picture);
        waitKey(100);
        if(squares.size()>=1)
        {
            waitKey(100);
            break;
        }
        
    }
    //destroyAllWindows();
}

void track_square(VideoCapture &video,vector<vector<Point> > &squares)
{
    //cout<<"tracking"<<endl;

    string trackerTypes[5] = {"MIL", "KCF",  "GOTURN",  "CSRT", "MOSSE"};
    string trackerType = trackerTypes[i_type];
    Mat frame;
    video.read(frame);
    int track_area=100;
    Rect_<int> bbox1(squares[0][0].x-track_area/2,squares[0][0].y-track_area/2,track_area,track_area);
    Tracker_ROI<tracker_type> track_roi_1(frame,trackerType,bbox1);//初始化
    Rect_<int> bbox2(squares[0][1].x-track_area/2,squares[0][1].y-track_area/2,track_area,track_area);
    Tracker_ROI< tracker_type> track_roi_2(frame,trackerType,bbox2);//初始化
    Rect_<int> bbox3(squares[0][2].x-track_area/2,squares[0][2].y-track_area/2,track_area,track_area);
    Tracker_ROI< tracker_type> track_roi_3(frame,trackerType,bbox3);//初始化
    Rect_<int> bbox4(squares[0][3].x-track_area/2,squares[0][3].y-track_area/2,track_area,track_area);
    Tracker_ROI< tracker_type> track_roi_4(frame,trackerType,bbox4);//初始化
    
    while (video.read(frame))
    {
        frame_num++;
        cout<<"tracking"<<endl;
        //std::cout<<"done here";
       track_roi_1.New_frame(frame);
       track_roi_2.New_frame(frame);
       track_roi_3.New_frame(frame);
       track_roi_4.New_frame(frame);
       //cout<<track_roi_1.track_status<<endl;
       if(!(track_roi_1.track_status&&track_roi_2.track_status&&track_roi_3.track_status&&track_roi_4.track_status)){
        //cout<<"lost tracking"<<endl;
        break;
       }
       
    }
}