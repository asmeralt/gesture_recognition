#include "VecClassFormatter.h"

VecClassFormatter::VecClassFormatter(int maxClassId) :requiredType(CV_32FC1), maxClassId(maxClassId)
{
}

VecClassFormatter::VecClassFormatter(int requiredType, int maxClassId): requiredType(requiredType), maxClassId(maxClassId)
{
}

cv::Mat VecClassFormatter::format(int classId) {
	cv::Mat classVec(1, maxClassId, requiredType, cv::Scalar(classId));
	classVec.at<float>(0, classId - 1) = UCHAR_MAX;
	return classVec;
}

int VecClassFormatter::formatBack(cv::Mat& classVec) {
	int maxIdx;
	cv::minMaxIdx(classVec, (double *)0, (double *)0, (int *)0, &maxIdx);
	return maxIdx;
}

int VecClassFormatter::getRequiredColumns() {
	return maxClassId;
}

int VecClassFormatter::getRequiredType() {
	return requiredType;
}

VecClassFormatter::~VecClassFormatter()
{
}
