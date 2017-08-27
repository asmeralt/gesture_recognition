#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter) : StatModelRecognizer(model, formatter)
{
}

Gesture GestureRecognizer::classVecToGesture(cv::Mat& prediction) {
	cv::Point maxIdx;
	cv::minMaxLoc(prediction, (double *)0, (double *)0, (cv::Point*)0, &maxIdx);
	switch (maxIdx.x) {
	case 0: return Gesture::PAPER;
	case 1: return Gesture::ROCK;
	case 2: return Gesture::SCISSORS;
	default: return Gesture::UNKNOWN;
	}
}

Gesture GestureRecognizer::classNumToGesture(cv::Mat& prediction) {
	switch ((int)prediction.data[0]) {
	case 0: return Gesture::PAPER;
	case 1: return Gesture::ROCK;
	case 2: return Gesture::SCISSORS;
	default: return Gesture::UNKNOWN;
	}
}

GestureRecognizer::~GestureRecognizer()
{
	StatModelRecognizer::~StatModelRecognizer();
}
