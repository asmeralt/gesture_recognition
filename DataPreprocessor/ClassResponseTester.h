#pragma once
#include "Tester.h"
class ClassResponseTester :
	public Tester
{
private:
	float classThreshold;
public:
	ClassResponseTester(cv::Ptr<cv::ml::StatModel> sm, float classThreshold);

	virtual float test(cv::Ptr<cv::ml::TrainData> data, const double precision);

	~ClassResponseTester();
private:
	void thresholdPredictions(cv::Mat& predictions);
};

