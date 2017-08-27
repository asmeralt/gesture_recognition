#include "RecognitionStateExecutor.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <iostream>

RecognitionStateExecutor::RecognitionStateExecutor(ColorMasker* colorMasker, GestureRecognizer* recognizer): colorMasker(colorMasker), recognizer(recognizer)
{
}

void RecognitionStateExecutor::execute(cv::Mat& frame) {
	ColorMasker masker;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2YCrCb);
	cv::Mat mask = masker.mask(frame, skinColorRange);
	cv::cvtColor(frame, frame, cv::COLOR_YCrCb2BGR);
	cv::imshow("Mask", mask);

	int squareSize = frame.rows / 2.2;
	int squareThickness = 2;
	cv::Rect alertRectangle(0, (frame.rows - squareSize) / 2, squareSize + 2 * squareThickness, squareSize + 2 * squareThickness);
	cv::rectangle(frame, alertRectangle, cv::Scalar(0, 255, 255), squareThickness, cv::LINE_4);
	cv::Rect roiRect(0, (frame.rows - squareSize) / 2 + squareThickness, squareSize, squareSize);

	cv::Mat roiMask = mask(roiRect);
	plotGesture(frame, recognizer->predictGesture(roiMask));

}

void RecognitionStateExecutor::setColorMasker(ColorMasker* colorMasker) {
	this->colorMasker = colorMasker;
}

void RecognitionStateExecutor::setRecognizer(GestureRecognizer* recognizer) {
	this->recognizer = recognizer;
}

void RecognitionStateExecutor::setSkinColorRange(std::vector<cv::Scalar>& skinColorRange) {
	this->skinColorRange = skinColorRange;
}

void RecognitionStateExecutor::plotDebugInfo(cv::Mat& frame) {
	//cv::text
}

void RecognitionStateExecutor::plotGesture(cv::Mat& frame, Gesture gesture) {
	cv::Scalar color;
	std::string gestureName;
	switch (gesture) {
	case Gesture::PAPER: gestureName = "Paper"; color = cv::Scalar(75, 255, 75); break;
	case Gesture::ROCK: gestureName = "Rock"; color = cv::Scalar(75, 75, 255); break;
	case Gesture::SCISSORS: gestureName = "Scissors"; color = cv::Scalar(255, 75, 75); break;
	default: gestureName = "UNKNOWN";  break;
	}
	cv::putText(frame, gestureName, cv::Point(frame.cols - 220, 40), cv::FONT_HERSHEY_PLAIN, 3, color, 3);
}

RecognitionStateExecutor::~RecognitionStateExecutor()
{
}
