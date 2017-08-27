#include "RecognitionStateExecutor.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>

RecognitionStateExecutor::RecognitionStateExecutor(ColorMasker* colorMasker, GestureRoiFinder* roiFinder, GestureRecognizer* recognizer): colorMasker(colorMasker), roiFinder(roiFinder), recognizer(recognizer)
{
}

void RecognitionStateExecutor::execute(cv::Mat& frame) {
	double ticks = (double)cv::getTickCount();
	cv::Mat mask = maskFrame(frame);
	cv::Rect roiRect = roiFinder->findGestureRoi(mask);
	cv::Mat roiMask = mask(roiRect);
	plotGestureRoi(frame, roiRect);
	plotDebugInfo(frame, ticks);
	plotGestureName(frame, recognizer->predictGesture(roiMask));
	cv::imshow("Mask", mask);
}

void RecognitionStateExecutor::setRecognizer(GestureRecognizer* recognizer) {
	this->recognizer = recognizer;
}

void RecognitionStateExecutor::initExecutor(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange) {
	this->skinColorRange = skinColorRange;
	cv::Mat frameCopy;
	cv::cvtColor(frame, frameCopy, cv::COLOR_BGR2YCrCb);
	this->colorMasker->setMaskBackground(frameCopy, skinColorRange);
}

cv::Mat RecognitionStateExecutor::maskFrame(cv::Mat& frame) {
	cv::Mat frameCopy;
	cv::cvtColor(frame, frameCopy, cv::COLOR_BGR2YCrCb);
	return colorMasker->mask(frameCopy, skinColorRange);
}

void RecognitionStateExecutor::plotGestureRoi(cv::Mat& frame, cv::Rect roiRect) {
	cv::rectangle(frame, roiRect, cv::Scalar(0, 255, 255), 2, cv::LINE_4);
}

void RecognitionStateExecutor::plotDebugInfo(cv::Mat& frame, double ticks) {
	double timeMillis = ((double)cv::getTickCount() - ticks) / cv::getTickFrequency() * 1000;
	cv::putText(frame, typeid(*(this->recognizer)).name(), cv::Point(frame.cols - 220, 80), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
	cv::putText(frame, cv::format("%5.3f, ms", timeMillis), cv::Point(frame.cols - 220, 100), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
}

void RecognitionStateExecutor::plotGestureName(cv::Mat& frame, Gesture gesture) {
	cv::Scalar color;
	std::string gestureName;
	switch (gesture) {
	case Gesture::PAPER: gestureName = "Paper"; color = cv::Scalar(75, 255, 75); break;
	case Gesture::ROCK: gestureName = "Rock"; color = cv::Scalar(75, 75, 255); break;
	case Gesture::SCISSORS: gestureName = "Scissors"; color = cv::Scalar(255, 75, 75); break;
	case Gesture::NONE: gestureName = "None"; color = cv::Scalar(255, 255, 255); break;
	default: gestureName = "UNKNOWN";  break;
	}
	cv::putText(frame, gestureName, cv::Point(frame.cols - 220, 40), cv::FONT_HERSHEY_PLAIN, 3, color, 3);
}

RecognitionStateExecutor::~RecognitionStateExecutor()
{
}
