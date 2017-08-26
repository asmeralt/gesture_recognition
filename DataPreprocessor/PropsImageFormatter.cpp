#include "PropsImageFormatter.h"
#include "FeatureProps.h"

PropsImageFormatter::PropsImageFormatter()
{
}

cv::Mat PropsImageFormatter::format(cv::Mat& image) {
	cv::Mat imageRow(1, 2, CV_32FC1);
	std::vector<cv::Point> contour = FeatureProps::findMaxContour(image);
	imageRow.at<float>(0, 0) = FeatureProps::calcSolidity(contour);
	imageRow.at<float>(0, 1) = FeatureProps::calcContourPerimeter(contour);
	return imageRow;
}

int PropsImageFormatter::getRequiredColumns() {
	return 2;
}

int PropsImageFormatter::getRequiredType() {
	return CV_32FC1;
}

PropsImageFormatter::~PropsImageFormatter()
{
}
