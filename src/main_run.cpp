//
// Created by zdl on 22-9-26.
//
#include<ros/ros.h>  
#include "ROI_tracker.h"
#include "opencv2/opencv.hpp"
#include "opencv2/tracking.hpp"
#include <opencv2/core/ocl.hpp>

using namespace cv;
using namespace std;
void dectect_square(VideoCapture &video,vector<vector<Point> > &squares);
void track_square(VideoCapture &video,vector<vector<Point> > &squares);
int main(int argc, char **argv)
{
   
   
    //Tracker_ROI track_roi(frame,trackerType);//初始化

    VideoCapture capture;
	capture.open("/home/zdl/catkin_ws/src/cross_frame/src/video1.webm");
	if (!capture.isOpened()) {
		printf("could not find video");
		return -1;
	}
    //测试时候注释调****************************************************8
    bool if_dectect=1;//跟踪(0)还是检测(1)
    vector<vector<Point> > squares;
	Mat frame,mask,frame_hsv,squares_picture;
    while(1)
    {
        // 检测框
        dectect_square(capture,squares);
        
        //  跟踪框    
       track_square(capture,squares);
        }

	

    //ros::init ( argc, argv,"cross_frame");  
    /*string trackerTypes[4] = {"MIL", "KCF",  "GOTURN",  "CSRT"};
    string trackerType = trackerTypes[1];
    VideoCapture video(0);
    if(!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }
    Mat frame;
    video.read(frame);
    Tracker_ROI track_roi(frame,trackerType);//初始化
    while (video.read(frame))
    {
        //std::cout<<"done here";
       track_roi.New_frame(frame);
    }
    */

    return 0;
}
// betaflight  mpu6050
//
void dectect_square(VideoCapture &video,vector<vector<Point> > &squares)
{
    squares.clear();
    Mat frame,mask,frame_hsv,squares_picture;
    while (video.read(frame)) 
    {   
        imshow("input_video", frame);
        cv::cvtColor(frame, frame_hsv,cv::COLOR_BGR2HSV);
		inRange(frame_hsv,Scalar(0.46*180,0.36*255,0.0*255),Scalar(0.6*180,0.7*255,1.0*255),mask);//过滤
        imshow("frame_hsv",mask);
       squares_picture=Mat::ones(mask.size(),mask.type())*255;
        
        findSquares( mask, squares,squares_picture);
        if(squares.size()==1)
        {
            waitKey(10);
            
            break;
        }
        
    }
    //destroyAllWindows();
}

void track_square(VideoCapture &video,vector<vector<Point> > &squares)
{

    string trackerTypes[4] = {"MIL", "KCF",  "GOTURN",  "CSRT"};
    string trackerType = trackerTypes[1];
    Mat frame;
    video.read(frame);
    int track_area=100;
    Rect_<int> bbox1(squares[0][0].x-track_area/2,squares[0][0].y-track_area/2,track_area,track_area);
    Tracker_ROI track_roi_1(frame,trackerType,bbox1);//初始化
    Rect_<int> bbox2(squares[0][1].x-track_area/2,squares[0][1].y-track_area/2,track_area,track_area);
    Tracker_ROI track_roi_2(frame,trackerType,bbox2);//初始化
    Rect_<int> bbox3(squares[0][2].x-track_area/2,squares[0][2].y-track_area/2,track_area,track_area);
    Tracker_ROI track_roi_3(frame,trackerType,bbox3);//初始化
    Rect_<int> bbox4(squares[0][3].x-track_area/2,squares[0][3].y-track_area/2,track_area,track_area);
    Tracker_ROI track_roi_4(frame,trackerType,bbox4);//初始化
    
    while (video.read(frame))
    {
        //cout<<"here"<<endl;
        //std::cout<<"done here";
       track_roi_1.New_frame(frame);
       track_roi_2.New_frame(frame);
       track_roi_3.New_frame(frame);
       track_roi_4.New_frame(frame);
       
    }
}