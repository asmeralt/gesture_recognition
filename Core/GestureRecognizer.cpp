#include "GestureRecognizer.h"

GestureRecognizer::GestureRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter) : StatModelRecognizer(model), formatter(formatter)
{
}

cv::Mat GestureRecognizer::predict(cv::Mat& image) {
	return StatModelRecognizer::predict(formatter->format(image));
}

Gesture GestureRecognizer::classVecToGesture(cv::Mat& prediction) {
	cv::Point maxIdx;
	cv::minMaxLoc(prediction, (double *)0, (double *)0, (cv::Point*)0, &maxIdx);
	switch (maxIdx.x) {
	case 0: return Gesture::PAPER;
	case 1: return Gesture::ROCK;
	case 2: return Gesture::SCISSORS;
	case 3: return Gesture::NONE;
	default: return Gesture::UNKNOWN;
	}
}

Gesture GestureRecognizer::classNumToGesture(cv::Mat& prediction) {
	switch ((int)prediction.data[0] - 1) {
	case 0: return Gesture::PAPER;
	case 1: return Gesture::ROCK;
	case 2: return Gesture::SCISSORS;
	case 3: return Gesture::NONE;
	default: return Gesture::UNKNOWN;
	}
}

GestureRecognizer::~GestureRecognizer()
{
	delete formatter;
}
