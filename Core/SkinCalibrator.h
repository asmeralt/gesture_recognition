#pragma once
#include "opencv2\core.hpp"

class SkinCalibrator {

public:
	std::vector<cv::Scalar> calibrate(cv::Mat & region, bool desiredChannels[], float alphas[], std::vector<cv::Mat> & histOut = std::vector<cv::Mat>(3));
	void plotHistogram(cv::Mat& histogram, cv::Scalar color, float alpha, cv::Mat& dst);
	void plotHistograms(std::vector<cv::Mat> histograms, std::vector<cv::Scalar> colors, float alphas[], cv::Mat& dst);

	~SkinCalibrator();
protected:
	void calcChannelHistogram(cv::Mat& channel, cv::Mat& histOut);
	void calcHistogramProperties(cv::Mat& histogram, int* mode, int* dispersion);
	std::vector<uchar> calcChannelRange(int mode, int dispertion, float alpha);
};


