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
   

    VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened()) {
		printf("could not find video");
		return -1;
	}
    //测试时候注释调****************************************************8

	Mat frame,mask,frame_hsv,squares;
	while (capture.read(frame)) 
    {   
        cout<<"?????";
        imshow("input_video", frame);
        cv::cvtColor(frame, frame_hsv,cv::COLOR_BGR2HSV);
		inRange(frame_hsv,Scalar(0.5*180,0.4*255,0.*255),Scalar(0.6*180,0.7*255,1.0*255),mask);//过滤
        imshow("frame_hsv",mask);
        squares=Mat::ones(mask.size(),mask.type())*255;
        findSquares( mask, squares);
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