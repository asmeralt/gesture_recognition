#pragma once
#include <opencv2\core.hpp>

class ColorMasker
{
public:
	ColorMasker();

	cv::Mat mask(cv::Mat& image, std::vector<cv::Scalar>& colorRange);

	~ColorMasker();
};

