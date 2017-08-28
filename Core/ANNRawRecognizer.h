#pragma once
#include "StatModelRecognizer.h"
#include "GestureRecognizer.h"
#include "RawImageFormatter.h"

class ANNRawRecognizer:
	public GestureRecognizer
{
public:
	ANNRawRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, RawImageFormatter* formatter);

	virtual Gesture predictGesture(cv::Mat& image);

	virtual ~ANNRawRecognizer();
};

