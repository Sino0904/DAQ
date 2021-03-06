/*

The MIT License

Copyright (c) 2015 Avi Singh

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"


#include <iostream>
#include <ctype.h>
#include <algorithm> // for copy
#include <vector>
#include <string>

using namespace cv;
using namespace std;

void featureTracking(Mat img_1, Mat img_2, vector<Point2f>& points1, vector<Point2f>& points2, vector<uchar>& status)	
{

	//this function automatically gets rid of points for which tracking fails

	vector<float> err;
	Size winSize=Size(11,11);
	TermCriteria termcrit=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.001);
	if (points1.size() > 0)
	{
		calcOpticalFlowPyrLK(img_1, img_2, points1, points2, status, err, winSize,1 , termcrit, 0, 0.0005);

		//getting rid of points for which the KLT tracking failed or those who have gone outside the frame
		int indexCorrection = 0;
		for( int i=0; i<status.size(); i++)
		{
			Point2f pt = points2.at(i- indexCorrection);
			if ((status.at(i) == 0)||(pt.x<0)||(pt.y<0))
			{
				if((pt.x<0)||(pt.y<0))
				{
					status.at(i) = 0;
				}
				points1.erase (points1.begin() + (i - indexCorrection));
				points2.erase (points2.begin() + (i - indexCorrection));
				indexCorrection++;
			}

		}
/*		cout << "Index Correction is : " << indexCorrection << endl;
		cout << "No. Of Features  is : " << points2.size() << endl;*/
	}
	else
	{
		cout << "Vector of feature points is empty !! " << endl;
	}
}

void Point2fToKeyPoint(const vector<Point2f>& Points,vector<KeyPoint> &KeyPoints)
{
	for(int i = 0; i < Points.size();i++)
	{
		KeyPoints.push_back(cv::KeyPoint(Points[i],1.f));
	}
}


void featureDetection(Mat &img_1, vector<Point2f>& points1)	
{
	vector<KeyPoint> keypoints_1;
/*	int max_corners = 200;
	double quality_measure = 0.1, min_distance  = 20;*/

	bool nonmaxSuppression = true;

//	goodFeaturesToTrack(img_1, keypoints_1, max_corners,quality_measure,min_distance,noArray(),15);
	FAST(img_1,keypoints_1,30,true);

//	Point2fToKeyPoint(points1,keypoints_1);
	KeyPoint::convert(keypoints_1,points1);
	cout << "Size of vector is : " << keypoints_1.size() << endl;
}


