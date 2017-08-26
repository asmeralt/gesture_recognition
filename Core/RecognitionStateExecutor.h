#pragma once
#include "StateExecutor.h"
#include "ColorMasker.h"
#include "GestureRecognizer.h"

class RecognitionStateExecutor:
	public StateExecutor
{
private:
	ColorMasker* colorMasker;
	GestureRecognizer* recognizer;
	std::vector<cv::Scalar> skinColorRange;
public:
	RecognitionStateExecutor(ColorMasker* colorMasker, GestureRecognizer* recognizer);

	virtual void execute(cv::Mat& frame);

	void setColorMasker(ColorMasker* colorMasker);

	void setRecognizer(GestureRecognizer* recognizer);

	void setSkinColorRange(std::vector<cv::Scalar>& skinColorRange);

	~RecognitionStateExecutor();
};

