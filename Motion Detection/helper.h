//
//  helper.h
//  testOPENCV
//
//  Created by Siddharth Sharma on 11/10/2015.
//  Copyright © 2015 Siddharth. All rights reserved.
//

#ifndef helper_h
#define helper_h

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <iomanip>
#include <ctime>

using namespace std;
using namespace cv;

//our sensitivity value to be used in the absdiff() function
//for higher sensitivity, use a lower value
const static int SENSITIVITY_VALUE = 40;

//size of blur used to smooth the intensity image output from absdiff() function
const static int BLUR_SIZE = 10;

//these two can be toggled by pressing 'd' or 't'
bool debugMode;
bool trackingEnabled;

//int to string helper function
string intToString(int number){
    
    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}

//time_t t = time(0);   // get time now
//struct tm * now = localtime( & t );
//cout << (now->tm_year + 1900) << '-'<< (now->tm_mon + 1) << '-'<<  now->tm_mday<< endl;

string getDateTime(){
    
    //get the system time
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    
    //create string to store the date and time
    string dateTime;
    
    //convert year to string
    string year = intToString(now->tm_year + 1900);
    
    std::stringstream m;
    m << setfill('0') << std::setw(2) << now->tm_mon + 1;
    string month = m.str();
    
    //day
    std::stringstream d;
    d<<std::setfill('0')<<std::setw(2)<< now->tm_mday;
    string day = d.str();
    
    //hour
    std::stringstream hr;
    hr<<setfill('0')<<std::setw(2)<<now->tm_hour + 1;
    string hour = hr.str();
    
    //minute
    std::stringstream min;
    min<<setfill('0')<<std::setw(2)<<now->tm_min;
    string minute = min.str();
    
    //second
    std::stringstream sec;
    sec<<setfill('0')<<std::setw(2)<<now->tm_sec;
    string second = sec.str();
    
    //here we use the year, month, day, hour, minute info to create a custom string
    //this will be displayed in the bottom left corner of our video feed.
    dateTime = year + "-" + month + "-" + day + "  " + hour + ":" + minute + ":" + second;
    
    return dateTime;
}
string getDateTimeForFile(){
    
    //this function does the exact same as "getDateTime()"
    //however it returns a string that can be used as a filename
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    string dateTime;
    
    string year = intToString(now->tm_year + 1900);
    
    std::stringstream m;
    m<<std::setfill('0')<<std::setw(2)<< now->tm_mon + 1;
    string month = m.str();
    
    std::stringstream d;
    d<<std::setfill('0')<<std::setw(2)<< now->tm_mday;
    string day = d.str();
    
    std::stringstream hr;
    hr<<setfill('0')<<std::setw(2)<<now->tm_hour;
    string hour = hr.str();
    
    std::stringstream min;
    min<<setfill('0')<<std::setw(2)<<now->tm_min;
    string minute = min.str();
    
    std::stringstream sec;
    sec<<setfill('0')<<std::setw(2)<<now->tm_sec;
    string second = sec.str();
    
    //here we use "_" instead of "-" and ":"
    //if we try to save a filename with "-" or ":" in it we will get an error.
    dateTime = year + "_" + month + "_" + day + "_" + hour + "h" + minute + "m" + second + "s";
    
    return dateTime;
}
bool detectMotion(Mat thresholdImage, Mat &cameraFeed){
    
    bool motionDetected = false;
    
    //create temp Mat for threshold image
    Mat temp;
    thresholdImage.copyTo(temp);
    
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    //find contours of filtered image using openCV findContours function
    //findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours
    
    //if contours vector is not empty, we have found some objects
    //we can simply say that if the vector is not empty, motion in the video feed has been detected.
    if(contours.size()>0)motionDetected=true;
    else motionDetected = false;
    
    return motionDetected;
    
}


#endif /* helper_h */
