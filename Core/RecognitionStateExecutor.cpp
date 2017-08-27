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
	cv::Rect roiRect = findRoi(mask);
	cv::Mat roiMask = mask(roiRect);
	plotGestureRoi(frame, roiRect);
	plotDebugInfo(frame, ticks);
	plotGestureName(frame, recognizeGesture(roiMask));
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

cv::Rect RecognitionStateExecutor::findRoi(cv::Mat& mask) {
	return roiFinder->findGestureRoi(mask);
}

Gesture RecognitionStateExecutor::recognizeGesture(cv::Mat& roiMask) {
	return recognizer->predictGesture(roiMask);
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
	cv::Scalar color = getGestureColor(gesture);
	std::string gestureName = getGestureName(gesture);
	cv::putText(frame, gestureName, cv::Point(frame.cols - 220, 40), cv::FONT_HERSHEY_PLAIN, 3, color, 3);
}

std::string RecognitionStateExecutor::getGestureName(Gesture gesture) {
	switch (gesture) {
	case Gesture::PAPER: return "Paper";
	case Gesture::ROCK: return "Rock";
	case Gesture::SCISSORS: return "Scissors";
	case Gesture::NONE: return "None";
	default: return "UNKNOWN";
	}
}

cv::Scalar RecognitionStateExecutor::getGestureColor(Gesture gesture) {
	switch (gesture) {
	case Gesture::PAPER: return cv::Scalar(75, 255, 75);
	case Gesture::ROCK: return cv::Scalar(75, 75, 255);
	case Gesture::SCISSORS: return cv::Scalar(255, 75, 75);
	case Gesture::NONE: return cv::Scalar(255, 255, 255);
	default: return cv::Scalar(255, 255, 255);
	}
}

RecognitionStateExecutor::~RecognitionStateExecutor()
{
}
