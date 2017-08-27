#include "NBCPropsRecognizer.h"

NBCPropsRecognizer::NBCPropsRecognizer(cv::Ptr<cv::ml::NormalBayesClassifier>& nbc, PropsImageFormatter* formatter): GestureRecognizer(nbc.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture NBCPropsRecognizer::predictGesture(cv::Mat& image) {
	return classNumToGesture(this->predict(image));
}

NBCPropsRecognizer::~NBCPropsRecognizer()
{
}
