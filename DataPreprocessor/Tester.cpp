#include "Tester.h"

Tester::Tester(cv::Ptr<cv::ml::StatModel> sm) :sm(sm) 
{
}

cv::Ptr<cv::ml::StatModel> Tester::getModel() {
	return sm;
}

Tester::~Tester() 
{
}