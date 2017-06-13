#include "CDataCollection.h"
#include <strstream>
using namespace cv;

Mat Result;

bool IsValid(Mat pic)
{
	//return true;
	bool valid;
	vector<Point2f> camPoint;
	Mat temp;
	pic.copyTo(temp);
	pic.copyTo(Result);
	resize(pic, temp, Size(640, 512));
	valid = findChessboardCorners(temp, Size(6, 9), camPoint, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
	drawChessboardCorners(temp, Size(6, 9), camPoint, valid);
	imshow("test", temp);
	waitKey(1000);
	return valid;
}

int main()
{
	bool status = true;
	
	CDataCollection dc;

	if (status)
	{
		status = dc.Init();
	}
	if (status)
	{
		status = dc.CollectData();
	}
	if (status)
	{
		status = dc.Close();
	}

	return 0;
}