#include "PropsImageFormatter.h"
#include "FeatureProps.h"

PropsImageFormatter::PropsImageFormatter(std::vector<cv::Point>(contourFinder)(cv::Mat&), std::vector<float(*)(std::vector<cv::Point>&)>& props) :
	contourFinder(contourFinder),
	props(props)
{
}

cv::Mat PropsImageFormatter::format(cv::Mat& image) {
	cv::Mat imageRow(1, 2, CV_32FC1);
	std::vector<cv::Point> contour = this->contourFinder(image);
	float* row = imageRow.ptr<float>(0);
	for (int i = 0; i < props.size(); ++i) {
		row[i] = props[i](contour);
	}
	return imageRow;
}

int PropsImageFormatter::getRequiredColumns() {
	return props.size();
}

int PropsImageFormatter::getRequiredType() {
	return CV_32FC1;
}

PropsImageFormatter::~PropsImageFormatter()
{
}
