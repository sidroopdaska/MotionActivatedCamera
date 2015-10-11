//
//  main.cpp
//  testOpevCV
//
//  Created by Siddharth Sharma on 11/10/2015.
//  Copyright © 2015 Siddharth. All rights reserved.
//
#include <iostream>
#include "helper.h"

int main(){
    bool recording = false;
    bool startNewRecording = false;
    int inc = 0;
    bool firstRun = true;
    bool motionDetected = false;
    bool pause = false;
    
    //set debug mode and trackingenabled initially to false
    //these can be toggled using 'd' and 't'
    debugMode = false;
    trackingEnabled = false;
    

    //the two frames we will be comparing
    Mat frame1,frame2;
    
    //their grayscale images (needed for absdiff() function)
    Mat grayImage1,grayImage2;
    
    //resulting difference image
    Mat differenceImage;
    
    //thresholded difference image (for use in findContours() function)
    Mat thresholdImage;
    
    //video capture object.
    VideoCapture capture;
    capture.open(0);
    VideoWriter oVideoWriter;
    double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    cout<< "Frame Size = " << dWidth << "x" << dHeight << endl;
    
    //set framesize for use with videoWriter
    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
    
    if(!capture.isOpened()){
        cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }
    while(1){
    
        if(startNewRecording==true){
            oVideoWriter  = VideoWriter("/Users/sid/Documents/"+intToString(inc)+".avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);
            recording = true;
            startNewRecording = false;
            cout<<"New video file created /Users/sid/Documentsn/MyVideo"+intToString(inc)+".avi "<<endl;
            
            if (!oVideoWriter.isOpened() )
            {
                cout << "ERROR: Failed to initialize video writing" << endl;
                getchar();
                return -1;
            }
        }
        
        capture.read(frame1);
        
        //convert frame1 to gray scale for frame differencing
        cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
        capture.read(frame2);
        
        //convert frame2 to gray scale for frame differencing
        cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
        
        //perform frame differencing with the sequential images. This will output an "intensity image"
        //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
        cv::absdiff(grayImage1,grayImage2,differenceImage);
        
        //threshold intensity image at a given sensitivity value
        cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
        
        if(debugMode==true){
            cv::imshow("Difference Image",differenceImage);
            cv::imshow("Threshold Image", thresholdImage);
        } else {
            cv::destroyWindow("Difference Image");
            cv::destroyWindow("Threshold Image");
        }
        
        //blur the image to get rid of the noise. This will output an intensity image
        cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
        
        //threshold again to obtain binary image from blur output
        cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
        if(debugMode==true){
            imshow("Final Threshold Image",thresholdImage);
            
        } else {
            cv::destroyWindow("Final Threshold Image");
        }
        
        //if tracking enabled, search for Motion
        if(trackingEnabled){
            motionDetected = detectMotion(thresholdImage,frame1);
        } else{
            recording = false;
            motionDetected = false;
        }
        
        //draw time stamp to video in bottom left corner.
        rectangle(frame1,Point(0,460),Point(200,480),Scalar(255,255,255),-1);
        putText(frame1,getDateTime(),Point(0,480),1,1,Scalar(0,0,0),2);
        
        if(recording){
            oVideoWriter.write(frame1);
            putText(frame1,"REC",Point(0,60),2,2,Scalar(0,0,255),2);
        }
        
        if(motionDetected){
            putText(frame1,"MOTION DETECTED",cv::Point(0,420),2,2,cv::Scalar(0,255,0));
            recording = true;
            
            if(firstRun == true){
                
                string videoFileName = "/Users/sid/Documents/"+getDateTimeForFile()+".avi";
                cout << "File has been opened for writing: " << videoFileName<<endl;
                oVideoWriter  = VideoWriter(videoFileName, CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);
                
                if ( !oVideoWriter.isOpened() )
                {
                    cout << "ERROR: Failed to initialize video writing" << endl;
                    getchar();
                    return -1;
                }
                firstRun = false;
                
                
            }
            
        }else recording = false;
        
        imshow("Frame1",frame1);
        
        //check to see if a button has been pressed.
        //this delay is necessary for proper operation of this program
        //if removed, frames will not have enough time to referesh and a blank
        //image will appear.
        switch(waitKey(30)){
                
            case 27: //'esc' key has been pressed, exit program.
                return 0;
            case 116: //'t' has been pressed. this will toggle tracking
                trackingEnabled = !trackingEnabled;
                if(trackingEnabled == false) cout<<"Tracking disabled."<<endl;
                else cout<<"Tracking enabled."<<endl;
                break;
            case 100: //'d' has been pressed. this will debug mode
                debugMode = !debugMode;
                if(debugMode == false) cout<<"Debug mode disabled."<<endl;
                else cout<<"Debug mode enabled."<<endl;
                break;
            case 112: //'p' has been pressed. this will pause/resume the code.
                pause = !pause;
                if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
                    while (pause == true){
                        //stay in this loop until 
                        switch (waitKey()){
                                //a switch statement inside a switch statement? Mind blown.
                            case 112: 
                                //change pause back to false
                                pause = false;
                                cout<<"Code Resumed"<<endl;
                                break;
                        }
                    }
                }
                
            case 114:
                //'r' has been pressed.
                //toggle recording mode
                recording =!recording;
                
                if(firstRun == true){
                    
                    cout << "New Recording Started" << endl;
                    oVideoWriter  = VideoWriter("/Users/sid/Documents/MyVideo0.avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);
                    
                    if ( !oVideoWriter.isOpened() ) 
                    {
                        cout << "ERROR: Failed to initialize video writing" << endl;
                        getchar();
                        return -1;
                    }
                    firstRun = false;
                    
                    
                }
                else {if (!recording)cout << "Recording Stopped" << endl;
                    
                else cout << "Recording Started" << endl;
                }
                break;
                
            case 110:
                //'n' has been pressed
                //start new video file
                startNewRecording = true;
                cout << "New Recording Started" << endl;
                inc+=1;
                break; 
                
        }
        
    }
    
    return 0;
    
}

//#include <iostream>
//#include <opencv2/highgui/highgui.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main(int argc, char* argv[])
//{
//    VideoCapture cap(0); // open the video camera no. 0
//    
//    if (!cap.isOpened())  // if not success, exit program
//    {
//        cout << "ERROR: Cannot open the video file" << endl;
//        return -1;
//    }
//    
//    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
//    
//    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
//    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
//    
//    cout << "Frame Size = " << dWidth << "x" << dHeight << endl;
//    
//    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));
//    
//    VideoWriter oVideoWriter ("/Users/sid/Documents/MyVideo.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object
//    
//    if ( !oVideoWriter.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
//    {
//        cout << "ERROR: Failed to write the video" << endl;
//        return -1;
//    }
//    
//    while (1)
//    {
//        
//        Mat frame;
//        
//        bool bSuccess = cap.read(frame); // read a new frame from video
//        
//        if (!bSuccess) //if not success, break loop
//        {
//            cout << "ERROR: Cannot read a frame from video file" << endl;
//            break;
//        }
//        
//        oVideoWriter.write(frame); //writer the frame into the file
//        
//        imshow("MyVideo", frame); //show the frame in "MyVideo" window
//        
//        if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//        {
//            cout << "esc key is pressed by user" << endl;
//            break;
//        }
//    }
//    
//    return 0;
//    
//}