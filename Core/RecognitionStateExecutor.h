#pragma once
#include "StateExecutor.h"
#include "ColorMasker.h"
#include "GestureRoiFinder.h"
#include "GestureRecognizer.h"

class RecognitionStateExecutor:
	public StateExecutor
{
private:
	ColorMasker* colorMasker;
	GestureRoiFinder* roiFinder;
	GestureRecognizer* recognizer;
	std::vector<cv::Scalar> skinColorRange;
public:
	RecognitionStateExecutor(ColorMasker* colorMasker, GestureRoiFinder* roiFinder, GestureRecognizer* recognizer);

	virtual void execute(cv::Mat& frame);

	void setRecognizer(GestureRecognizer* recognizer);

	void initExecutor(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange);

	~RecognitionStateExecutor();
private:
	cv::Mat maskFrame(cv::Mat& frame);

	void plotGestureRoi(cv::Mat& frame, cv::Rect roiRect);

	void plotDebugInfo(cv::Mat& frame, double ticks);
	
	void plotGestureName(cv::Mat& frame, Gesture gesture);
};

