#pragma once
#include <opencv2\core.hpp>

class ColorMasker
{
public:
	ColorMasker();

	virtual cv::Mat mask(cv::Mat& image, std::vector<cv::Scalar>& colorRange);

	virtual ~ColorMasker();
private:
	cv::Mat findRawMask(cv::Mat& image, std::vector<cv::Scalar>& colorRange);
};

