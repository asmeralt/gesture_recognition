#include "NumClassFormatter.h"

NumClassFormatter::NumClassFormatter() :requiredType(CV_32FC1)
{
}

NumClassFormatter::NumClassFormatter(int requiredType): requiredType(requiredType)
{

}

cv::Mat NumClassFormatter::format(int classId) {
	return cv::Mat(1, 1, requiredType, cv::Scalar(classId));
}

int NumClassFormatter::formatBack(cv::Mat& classVec) {
	return (int)classVec.data[0];
}

int NumClassFormatter::getRequiredColumns() {
	return 1;
}

int NumClassFormatter::getRequiredType() {
	return requiredType;
}

NumClassFormatter::~NumClassFormatter()
{
}
