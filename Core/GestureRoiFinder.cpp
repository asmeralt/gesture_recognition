#include "GestureRoiFinder.h"
#include <opencv2\imgproc.hpp>

GestureRoiFinder::GestureRoiFinder()
{
}

cv::Rect GestureRoiFinder::findGestureRoi(cv::Mat& mask) {
	int squareSize = mask.rows / 2.2;
	return cv::Rect(0, (mask.rows - squareSize) / 2, squareSize, squareSize);
}

GestureRoiFinder::~GestureRoiFinder()
{
}
