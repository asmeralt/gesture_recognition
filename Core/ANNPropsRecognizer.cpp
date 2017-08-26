#include "ANNPropsRecognizer.h"

ANNPropsRecognizer::ANNPropsRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, PropsImageFormatter* formatter): GestureRecognizer(ann.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture ANNPropsRecognizer::predictGesture(cv::Mat& image) {
	return this->classVecToGesture(this->predict(image));
}

ANNPropsRecognizer::~ANNPropsRecognizer()
{
}
