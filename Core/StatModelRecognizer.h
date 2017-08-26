#pragma once
#include "Recognizer.h"
#include "SampleFormatter.h"
#include <opencv2\ml.hpp>

class StatModelRecognizer:
	public Recognizer
{
protected:
	cv::Ptr<cv::ml::StatModel> model;

	ImageFormatter* formatter;
public:
	StatModelRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter);

	virtual cv::Mat predict(cv::Mat& image);

	~StatModelRecognizer();
};

