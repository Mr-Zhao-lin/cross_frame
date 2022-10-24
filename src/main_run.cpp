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
int main(int argc, char **argv)
{
   
    string trackerTypes[4] = {"MIL", "KCF",  "GOTURN",  "CSRT"};
    string trackerType = trackerTypes[1];
    //Tracker_ROI track_roi(frame,trackerType);//初始化





    VideoCapture capture;
	capture.open("/home/zdl/catkin_ws/src/cross_frame/src/video1.webm");
	if (!capture.isOpened()) {
		printf("could not find video");
		return -1;
	}
    //测试时候注释调****************************************************8
    bool if_dectect=1;//跟踪(0)还是检测(1)
	Mat frame,mask,frame_hsv,squares_picture;
	while (capture.read(frame)) 
    {   
        //cout<<"?????";
        imshow("input_video", frame);
        cv::cvtColor(frame, frame_hsv,cv::COLOR_BGR2HSV);
		inRange(frame_hsv,Scalar(0.46*180,0.36*255,0.0*255),Scalar(0.6*180,0.7*255,1.0*255),mask);//过滤
        imshow("frame_hsv",mask);
       squares_picture=Mat::ones(mask.size(),mask.type())*255;
        vector<vector<Point> > squares;
        
        if(if_dectect)
        {
            findSquares( mask, squares,squares_picture);
            //if(squares.size()==1)
            //{
            //    if_dectect=0;

            //}
        }
		//imshow("InRange_video", squares);
		//rectangle(frame, roi, Scalar(0, 255), 3, 8, 0);
		char c = waitKey(10);
		if (c == 27) {
			break;     
		}
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