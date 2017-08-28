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

	virtual void setRecognizer(GestureRecognizer* recognizer);

	virtual void initExecutor(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange);

	virtual ~RecognitionStateExecutor();
protected:
	cv::Mat maskFrame(cv::Mat& frame);

	cv::Rect findRoi(cv::Mat& mask);

	Gesture recognizeGesture(cv::Mat& roiMask);

	void plotGestureRoi(cv::Mat& frame, cv::Rect roiRect);

	void plotDebugInfo(cv::Mat& frame, double ticks);
	
	void plotGestureName(cv::Mat& frame, Gesture gesture);

	std::string getGestureName(Gesture gesture);

	cv::Scalar getGestureColor(Gesture gesture);
};

