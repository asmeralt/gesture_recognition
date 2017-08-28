#pragma once
#include <opencv2\core.hpp>

class GestureRoiFinder
{
public:
	GestureRoiFinder();

	virtual cv::Rect findGestureRoi(cv::Mat& mask);

	virtual ~GestureRoiFinder();
};

