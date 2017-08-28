#include "NBCPropsRecognizer.h"
#include "no_contours_exception.h"

NBCPropsRecognizer::NBCPropsRecognizer(cv::Ptr<cv::ml::NormalBayesClassifier>& nbc, PropsImageFormatter* formatter) : GestureRecognizer(nbc.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture NBCPropsRecognizer::predictGesture(cv::Mat& image) {
	try {
		return classNumToGesture(this->predict(image));
	}
	catch (no_contours_exception exc) {
		return Gesture::NONE;
	}
}

NBCPropsRecognizer::~NBCPropsRecognizer()
{
	GestureRecognizer::~GestureRecognizer();
}
