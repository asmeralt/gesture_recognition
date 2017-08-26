#pragma once
#include <opencv2\ml.hpp>

class Tester
{
protected:
	cv::Ptr<cv::ml::StatModel> sm;
public:
	Tester(cv::Ptr<cv::ml::StatModel> sm);

	virtual float test(cv::Ptr<cv::ml::TrainData> data, const double precision) = 0;

	virtual cv::Ptr<cv::ml::StatModel> getModel();

	~Tester();
};

