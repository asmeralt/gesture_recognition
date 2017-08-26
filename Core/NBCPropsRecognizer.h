#pragma once
#include "StatModelRecognizer.h"
#include "GestureRecognizer.h"
#include "PropsImageFormatter.h"

class NBCPropsRecognizer:
	public GestureRecognizer
{
public:
	NBCPropsRecognizer(cv::Ptr<cv::ml::NormalBayesClassifier>& nbc, PropsImageFormatter* formatter);

	virtual Gesture predictGesture(cv::Mat& image);

	~NBCPropsRecognizer();
};

