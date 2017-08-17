#pragma once
#include <opencv2\core.hpp>
#include "VideoDispatcherState.h"
class StateExecutor
{
public:
	virtual void execute(cv::Mat& frame) = 0;
};

