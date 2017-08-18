#pragma once
#include "VideoDispatcherState.h"
#include "SkinCalibrator.h"
#include <string>
#include <opencv2\core.hpp>
#include "CalibrationStateExecutor.h"

class VideoDispatcher
{
private:
	std::string windowName;
	int frameCaptureDelayMillis;

	/* State machine */
	VideoDispatcherState state;
	CalibrationStateExecutor& calibration;



	int exampleId = 0;
	bool save = false;

public:
	VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, CalibrationStateExecutor& calibration);

	void run();

	~VideoDispatcher();

private:

	void recognition(cv::Mat& frame);

};

