#include "ANNPropsRecognizer.h"
#include "no_contours_exception.h"

ANNPropsRecognizer::ANNPropsRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, PropsImageFormatter* formatter) : GestureRecognizer(ann.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture ANNPropsRecognizer::predictGesture(cv::Mat& image) {
	try {
		return this->classVecToGesture(this->predict(image));
	}
	catch (no_contours_exception exc) {
		return Gesture::NONE;
	}
}

ANNPropsRecognizer::~ANNPropsRecognizer()
{
}
