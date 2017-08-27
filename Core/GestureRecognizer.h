#pragma once
#include <opencv2\core.hpp>
#include "Gesture.h"
#include "StatModelRecognizer.h"

class GestureRecognizer:
	public StatModelRecognizer
{
public:
	GestureRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter);

	virtual Gesture predictGesture(cv::Mat& image) = 0;

	~GestureRecognizer();
protected:
	Gesture classVecToGesture(cv::Mat& prediction);

	Gesture classNumToGesture(cv::Mat& prediction);
};

