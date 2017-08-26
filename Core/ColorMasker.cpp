#include "ColorMasker.h"
#include <opencv2\imgproc.hpp>

ColorMasker::ColorMasker()
{
}

cv::Mat ColorMasker::mask(cv::Mat& image, std::vector<cv::Scalar> colorRange) {
	cv::Mat mask;
	cv::Mat blurredImage;
	cv::medianBlur(image, blurredImage, 9);
	cv::inRange(blurredImage, colorRange[0], colorRange[1], mask);
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 5);
	return mask;
}

ColorMasker::~ColorMasker()
{
}
