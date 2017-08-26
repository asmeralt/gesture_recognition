#pragma once
#include "Tester.h"
class NumResponseTester :
	public Tester
{

public:
	NumResponseTester(cv::Ptr<cv::ml::StatModel> sm);

	virtual float test(cv::Ptr<cv::ml::TrainData> data, const double precision);

	~NumResponseTester(); 
private:
	double calcEuclideanNorm(float* row, int length);
};

