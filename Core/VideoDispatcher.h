#pragma once
#include "VideoDispatcherState.h"
#include <string>
#include <opencv2\core.hpp>

class VideoDispatcher
{
private:
	std::string windowName;
	int frameCaptureDelayMillis;
	VideoDispatcherState state;
	
public:
	VideoDispatcher(std::string windowName, int frameCaptureDelayMillis);

	void run();

	~VideoDispatcher();

private:
	void calibration(cv::Mat& frame);

	void recognition(cv::Mat& frame);
};

