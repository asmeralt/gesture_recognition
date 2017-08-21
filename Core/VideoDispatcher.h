#pragma once
#include <string>
#include <opencv2\core.hpp>
#include <opencv2\ml.hpp>
#include "VideoDispatcherState.h"
#include "SkinCalibrator.h"
#include "CalibrationStateExecutor.h"

class VideoDispatcher
{
private:
	std::string windowName;
	int frameCaptureDelayMillis;

	/* State machine */
	VideoDispatcherState state;
	CalibrationStateExecutor& calibration;

	cv::Ptr<cv::ml::ANN_MLP> ann;

public:
	VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, CalibrationStateExecutor& calibration, cv::Ptr<cv::ml::ANN_MLP> ann);

	void run();

	~VideoDispatcher();

private:

	void recognition(cv::Mat& frame);

};

