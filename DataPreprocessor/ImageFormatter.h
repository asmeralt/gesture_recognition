#pragma once
#include <opencv2\core.hpp>

class ImageFormatter
{
public:
	ImageFormatter();

	virtual cv::Mat format(cv::Mat& image) = 0;

	virtual int getRequiredColumns() = 0;

	virtual int getRequiredType() = 0;

	virtual ~ImageFormatter();
};

