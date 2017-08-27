#pragma once
#include <opencv2\core.hpp>

class ColorMasker
{
protected:
	cv::Mat maskBackground;
public:
	ColorMasker();

	virtual cv::Mat mask(cv::Mat& image, std::vector<cv::Scalar>& colorRange);

	virtual void setMaskBackground(cv::Mat& image, std::vector<cv::Scalar>& colorRange);

	~ColorMasker();
private:
	cv::Mat findRawMask(cv::Mat& image, std::vector<cv::Scalar>& colorRange);
};

