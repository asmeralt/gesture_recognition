#pragma once
#include "StatModelRecognizer.h"
#include "GestureRecognizer.h"
#include "PropsImageFormatter.h"

class ANNPropsRecognizer:
	public GestureRecognizer
{
public:
	ANNPropsRecognizer(cv::Ptr<cv::ml::ANN_MLP>& ann, PropsImageFormatter* formatter);

	virtual Gesture predictGesture(cv::Mat& image);

	~ANNPropsRecognizer();
};

