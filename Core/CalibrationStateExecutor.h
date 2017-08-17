#pragma once
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include "StateExecutor.h"
#include "SkinCalibrator.h"

class CalibrationStateExecutor: public StateExecutor
{
private:
	float alpha;
	int alphaTrackbarValue;
	const int alphaTrackbarMax;
	SkinCalibrator skinCalibrator;
	std::vector<cv::Scalar> skinColorFilterRange;

public:
	CalibrationStateExecutor(SkinCalibrator& skinCalibrator, float alpha = 1.0f);
	~CalibrationStateExecutor();

	void execute(cv::Mat& frame);
	std::vector<cv::Scalar> getSkinColorFilterRange();

private:
	cv::Rect buildAndPlotAlertRectangle(cv::Mat&  frame);
	void showCalibrationHistograms(std::vector<cv::Mat>& histograms);
	void plotCalibrationHistograms(std::vector<cv::Mat> histograms, cv::Mat& dst);
};

