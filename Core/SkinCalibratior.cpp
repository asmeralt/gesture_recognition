#include "SkinCalibrator.h"
#include "opencv2\imgproc.hpp"

std::vector<cv::Scalar> SkinCalibrator::calibrate(cv::Mat & region, bool desiredChannels[], float alpha[], std::vector<cv::Mat> & histOut)
{
	cv::Mat calibrationRegion = region.clone();
	std::vector<cv::Mat> channels;
	cv::split(calibrationRegion, channels);
	cv::Scalar lower;
	cv::Scalar upper;
	for (int i = 0; i < 3; ++i) {
		if (desiredChannels[i]) {
			cv::Mat chHist;
			int mode, variance;
			calcChannelHistogram(channels[i], chHist);
			calcHistogramProperties(chHist, &mode, &variance);
			std::vector<uchar> chRange = calcChannelRange(mode, variance, alpha[i]);
			lower[i] = chRange[0];
			upper[i] = chRange[1];
			histOut[i] = chHist;
		}
		else {
			lower[i] = 0;
			upper[i] = UCHAR_MAX;
		}
	}
	std::vector<cv::Scalar> range(2);
	range[0] = lower;
	range[1] = upper;
	return range;
}

void SkinCalibrator::plotHistogram(cv::Mat& histogram, cv::Scalar color, float alpha, cv::Mat& dst) {
	int histSize = histogram.rows;
	int bin_w = cvRound((double)dst.cols / histSize);
	cv::Mat normHist(dst.rows, 1, dst.type());
	cv::normalize(histogram, normHist, 0, dst.rows, cv::NORM_MINMAX, -1, cv::Mat());
	int mode, variance;
	calcHistogramProperties(histogram, &mode, &variance);
	cv::line(dst, cv::Point(bin_w * (mode - alpha * std::sqrt(variance)),0), cv::Point(bin_w * (mode - alpha * std::sqrt(variance)), dst.cols), color / 2, 1);
	cv::line(dst, cv::Point(bin_w * (mode + alpha * std::sqrt(variance)),0), cv::Point(bin_w * (mode + alpha * std::sqrt(variance)), dst.cols), color / 2, 1);

	for (int i = 1; i < histSize; i++)
	{
		line(dst, cv::Point(bin_w*(i - 1), dst.rows - cvRound(normHist.at<float>(i - 1))),
			cv::Point(bin_w*(i), dst.rows - cvRound(normHist.at<float>(i))),
			color, 2, 8, 0);
	}
}

void SkinCalibrator::plotHistograms(std::vector<cv::Mat>& histograms, std::vector<cv::Scalar>& colors, float alphas[], cv::Mat& dst) {
	for (int i = 0; i < histograms.size(); ++i) {
		plotHistogram(histograms[i], colors[i], alphas[i], dst);
	}
}

void SkinCalibrator::calcChannelHistogram(cv::Mat& channel, cv::Mat& histOut) {
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;

	cv::calcHist(&channel, 1, 0, cv::Mat(), histOut, 1, &histSize, &histRange, uniform, accumulate);
}

void SkinCalibrator::calcHistogramProperties(cv::Mat& histogram, int* mode, int* variance) {
	if (histogram.cols == 0) {
		*mode = 0;
		*variance = 0;
		return;
	}
	double min, max;
	cv::Point minIdx, maxIdx;

	cv::minMaxLoc(histogram, &min, &max, &minIdx, &maxIdx); // Ask about effiency of adding a mask!!!!

	double vari = 0;
	float* histVal = histogram.col(0).ptr<float>();
	int totalVal = histogram.rows * histogram.cols;

	for (int i = 0; i < histogram.rows; ++i) {
		vari += std::pow(i - maxIdx.y, 2) * histVal[i] / totalVal;
	}

	*mode = maxIdx.y;
	*variance = std::ceil(vari);
}

std::vector<uchar> SkinCalibrator::calcChannelRange(int mode, int variance, float alpha) {
	std::vector<uchar> range(2);
	range[0] = mode - alpha * std::sqrt(variance);
	range[1] = mode + alpha * std::sqrt(variance);
	return range;
}

SkinCalibrator::~SkinCalibrator()
{
}
