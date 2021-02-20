#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
using namespace cv;
using namespace std;
Mat src_input;
Mat src_hsv;
Mat src_binary;
Mat thresh_binary;
Mat canny_output;
int thresh=10;
int main()
{
   VideoCapture cap(0);
   if(!cap.isOpened())
      return -1;
   while(1)
   {
       cap >> src_input;
       imshow("frame", src_input);
       cvtColor(src_input, src_hsv, CV_BGR2HSV);
       //imshow("hsv ", src_hsv);
       inRange(src_hsv, Scalar(0,60,30), Scalar(179, 255, 255), src_binary);
       line( src_binary, {0, src_input.rows}, {src_input.cols, src_input.rows}, Scalar( 0, 0, 0 ), 380, LINE_8);
       threshold(src_binary, src_binary, 30, 255, CV_THRESH_BINARY);
       //imshow("hsv_threshold ", src_binary);
       GaussianBlur(src_binary,src_binary, Size(1,1), 0);
       medianBlur(src_binary, src_binary, 7);
       dilate(src_binary, src_binary, Mat(), Point(-1, -1), 3);
       erode(src_binary,src_binary,Mat(), Point(-1, -1), 3);
       erode(src_binary,src_binary,Mat(), Point(-1, -1), 3);
       dilate(src_binary, src_binary, Mat(), Point(-1, -1), 3);
       imshow("Binary view", src_binary);
       Canny( src_binary, canny_output, thresh, thresh*2, 3 );
       std::vector<std::vector<cv::Point> > contours;

       findContours(src_binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
       vector<RotatedRect> smallRect( contours.size() );
       //vector<RotatedRect> minEllipse( contours.size() );
       for (double k = 0; k < (contours.size()); k++)
       {
           double area = contourArea(contours[k]);
                 if (area>900)
              smallRect[k] = minAreaRect( Mat(contours[k]) );
           drawContours(src_input, contours, k, Scalar(0, 255, 0), 1,8, vector <Vec4i>(),0,Point());
           imshow("contours",src_input );
       }
       Mat draw = Mat::zeros( src_input.size(), CV_8UC3 );
        for( int k = 0; k< int (contours.size()); k++ )
           {
            double area_cont = contourArea(contours[k]);
            if (area_cont>900)
             drawContours( src_input, contours, k, Scalar( 0, 255, 0 ), 1, 8, vector<Vec4i>(), 0, Point() );
            Point2f rectangle_points[4];
            smallRect[k].points( rectangle_points );
            double height = smallRect[k].size.height;
            double width = smallRect[k].size.width;
            if(height>width){
            smallRect[k].size.height =(float)(0.35)*smallRect[k].size.height;
            smallRect[k].center = (rectangle_points[1]+rectangle_points[2])/2 + (rectangle_points[0]-rectangle_points[1])/6;
            }
            else
            {
            smallRect[k].size.width =(float)(0.35)*smallRect[k].size.width;
            smallRect[k].center = (rectangle_points[2]+rectangle_points[3])/2 + (rectangle_points[0]-rectangle_points[3])/6;
            }
            smallRect[k].points( rectangle_points );
            for( int j = 0; j < 4; j++ )
            line( src_input, rectangle_points[j], rectangle_points[(j+1)%4], Scalar(0, 0, 0), 2, 8 );
            }
            imshow( "fingertip detection", src_input );

      if(waitKey(1) >= 0) break;
  }



}






