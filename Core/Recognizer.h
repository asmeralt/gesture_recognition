#pragma once
#include <opencv2\core.hpp>

class Recognizer
{
public:
	virtual cv::Mat predict(cv::Mat& data) = 0;
};

