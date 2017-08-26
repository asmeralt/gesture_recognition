#pragma once
#include "SampleFormatter.h"
class PropsSampleFormatter :
	public SampleFormatter
{
private:
	const int maxClassId;
public:
	PropsSampleFormatter(ClassFormatter* classFormatter, int maxClassId);

	virtual cv::Ptr<cv::ml::TrainData> format(std::vector<Sample> samples);

	virtual cv::Mat format(cv::Mat& sample);

	virtual ~PropsSampleFormatter();
};

