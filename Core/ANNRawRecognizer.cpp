#include "ANNRawRecognizer.h"

ANNRawRecognizer::ANNRawRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, RawImageFormatter* formatter) : GestureRecognizer(ann.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture ANNRawRecognizer::predictGesture(cv::Mat& image) {
	return this->classVecToGesture(this->predict(image));
}

ANNRawRecognizer::~ANNRawRecognizer()
{
}
