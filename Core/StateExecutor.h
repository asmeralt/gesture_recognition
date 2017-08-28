#pragma once
#include <opencv2\core.hpp>
#include "VideoDispatcherState.h"
class StateExecutor
{
public:
	StateExecutor();

	virtual void execute(cv::Mat& frame) = 0;

	virtual ~StateExecutor();
};
