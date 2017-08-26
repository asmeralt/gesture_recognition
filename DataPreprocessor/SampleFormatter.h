#pragma once
#include <opencv2\ml.hpp>
#include "Sample.h"
#include "ClassFormatter.h"

class SampleFormatter
{
protected:
	ClassFormatter* classFormatter;
public:
	SampleFormatter(ClassFormatter* classFormatter);

	virtual cv::Ptr<cv::ml::TrainData> format(std::vector<Sample> samples) = 0;

	virtual cv::Mat format(cv::Mat& sample) = 0;

	virtual ~SampleFormatter();
};

