cmake_minimum_required(VERSION 3.0.2)
set(OpenCV_DIR usr/local/include/opencv4/)

find_package(OpenCV REQUIRED)

include_directories(${Opencv_INCLUDE_DIRS} )
 add_library(ROI_tracker ROI_tracker.cpp  ROI_tracker.h)
 target_link_libraries(ROI_tracker ${OpenCV_LIBS}  )
