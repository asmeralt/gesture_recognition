#include "ANNRawRecognizer.h"
#include "no_contours_exception.h"

ANNRawRecognizer::ANNRawRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, RawImageFormatter* formatter) : GestureRecognizer(ann.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture ANNRawRecognizer::predictGesture(cv::Mat& image) {
	try {
		return this->classVecToGesture(this->predict(image));
	}
	catch (no_contours_exception exc) {
		return Gesture::NONE;
	}
}

ANNRawRecognizer::~ANNRawRecognizer()
{
	GestureRecognizer::~GestureRecognizer();
}
