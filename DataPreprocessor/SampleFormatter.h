#pragma once
#include <opencv2\ml.hpp>
#include "Sample.h"
#include "ImageFormatter.h"
#include "ClassFormatter.h"

class SampleFormatter
{
protected:
	ImageFormatter* imageFormatter;
	ClassFormatter* classFormatter;
public:
	SampleFormatter(ImageFormatter* imageFormatter, ClassFormatter* classFormatter);

	virtual cv::Ptr<cv::ml::TrainData> format(std::vector<Sample>& samples);

	virtual ~SampleFormatter();
};

