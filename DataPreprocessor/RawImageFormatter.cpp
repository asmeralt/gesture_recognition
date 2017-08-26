#include "RawImageFormatter.h"
#include <opencv2\imgproc.hpp>

RawImageFormatter::RawImageFormatter(cv::Size size): size(size)
{
}

cv::Mat RawImageFormatter::format(cv::Mat& image) {
	cv::Mat imageRow;
	image.convertTo(imageRow, CV_32FC1);
	cv::resize(imageRow, imageRow, this->size);
	return imageRow.reshape(0, 1);
}

int RawImageFormatter::getRequiredColumns() {
	return this->size.height * this->size.width;
}

int RawImageFormatter::getRequiredType() {
	return CV_32FC1;
}

RawImageFormatter::~RawImageFormatter()
{
}
