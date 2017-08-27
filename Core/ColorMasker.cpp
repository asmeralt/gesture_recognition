#include "ColorMasker.h"
#include <opencv2\imgproc.hpp>

ColorMasker::ColorMasker()
{
}

cv::Mat ColorMasker::mask(cv::Mat& image, std::vector<cv::Scalar>& colorRange) {
	return findRawMask(image, colorRange);
}

void ColorMasker::setMaskBackground(cv::Mat& image, std::vector<cv::Scalar>& colorRange) {
	this->maskBackground = findRawMask(image, colorRange);
	cv::morphologyEx(maskBackground, maskBackground, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)),cv::Point(-1,-1), 5);
}

ColorMasker::~ColorMasker()
{
}

cv::Mat ColorMasker::findRawMask(cv::Mat& image, std::vector<cv::Scalar>& colorRange) {
	cv::Mat mask;
	cv::Mat blurredImage;
	cv::medianBlur(image, blurredImage, 9);
	cv::inRange(blurredImage, colorRange[0], colorRange[1], mask);
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 5);
	return mask;
}
