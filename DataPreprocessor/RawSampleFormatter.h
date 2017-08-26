#pragma once
#include "SampleFormatter.h"
#include <opencv2\imgproc.hpp>

class RawSampleFormatter: public SampleFormatter
{
private:
	const cv::Size size;
	const int maxClassId;
public:
	RawSampleFormatter(ClassFormatter* classFormatter, const cv::Size size, const int maxClassId);

	virtual cv::Ptr<cv::ml::TrainData> format(std::vector<Sample> samples);

	virtual cv::Mat format(cv::Mat& sample);

	virtual ~RawSampleFormatter();
};

