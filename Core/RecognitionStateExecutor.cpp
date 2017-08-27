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
	Gesture gesture = recognizer->predictGesture(roiMask);

	switch (gesture) {
	case 0: std::cout << "Paper" << std::endl; break;
	case 1: std::cout << "Rock" << std::endl; break;
	case 2: std::cout << "Scissors" << std::endl; break;
	}
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

void displayDebugInfo(cv::Mat& frame) {
	//cv::text
}

RecognitionStateExecutor::~RecognitionStateExecutor()
{
}
