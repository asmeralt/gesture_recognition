#include "NBCPropsRecognizer.h"

NBCPropsRecognizer::NBCPropsRecognizer(cv::Ptr<cv::ml::NormalBayesClassifier>& nbc, PropsImageFormatter* formatter): GestureRecognizer(nbc.dynamicCast<cv::ml::StatModel>(), formatter)
{
}

Gesture NBCPropsRecognizer::predictGesture(cv::Mat& image) {
	cv::Mat prediction = this->predict(image);
	return Gesture::PAPER;
}

NBCPropsRecognizer::~NBCPropsRecognizer()
{
}
