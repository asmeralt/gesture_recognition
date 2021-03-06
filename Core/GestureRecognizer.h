#pragma once
#include <opencv2\core.hpp>
#include "Gesture.h"
#include "StatModelRecognizer.h"

class GestureRecognizer:
	public StatModelRecognizer
{
protected:
	ImageFormatter* formatter;
public:
	GestureRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter);

	virtual cv::Mat predict(cv::Mat& image);

	virtual Gesture predictGesture(cv::Mat& image) = 0;

	virtual ~GestureRecognizer();
protected:
	Gesture classVecToGesture(cv::Mat& prediction);

	Gesture classNumToGesture(cv::Mat& prediction);
};

