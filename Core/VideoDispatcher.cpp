#include "VideoDispatcher.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>

// public
VideoDispatcher::VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, CalibrationStateExecutor& calibrationStateExecutor, cv::Ptr<cv::ml::ANN_MLP> ann) :
	windowName(windowName),
	frameCaptureDelayMillis(frameCaptureDelayMillis),
	state(VD_CALIBRATION),
	calibration(calibrationStateExecutor),
	ann(ann)
{
}

VideoDispatcher::~VideoDispatcher()
{
}

void VideoDispatcher::run() {
	cv::VideoCapture webcam;
	cv::Mat frame;

	webcam.open(0);
	if (!webcam.isOpened()) { // check whereas webcam is available
		throw std::exception("Can not open a video stream from your webcam");
	}

	cv::namedWindow(windowName);

	while (state != VD_EXIT) {
		webcam >> frame; // reading webcam
		if (frame.empty() || !webcam.isOpened()) { // check if webcam stream is not emtpy or closed
			throw std::exception("Can not read a video stream from your webcam");
		}

		switch (state) {
		case VD_CALIBRATION: calibration.execute(frame); break;
		case VD_RECOGNITION: recognition(frame); break;
		case VD_EXIT: return;
		default: throw std::exception("Ooops, given dispatcher state %i is not implemented yet");
		}

		cv::imshow(windowName, frame); // display resulted frame

		// Break "while loop" when Esc is pressed, start calibration when 'c' is pressed, begin recognition when 'r' is pressed
		switch (cv::waitKey(frameCaptureDelayMillis)) {
		case 27: state = VD_EXIT; break; // Esc key code is 27
		case 'c': state = VD_CALIBRATION; break;
		case 'r': state = VD_RECOGNITION; break;
		default: break;
		}
	}
}

// private
void VideoDispatcher::recognition(cv::Mat& frame) {
	cv::Mat mask;
	cv::Mat blurredFrame;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2YCrCb);
	cv::medianBlur(frame, blurredFrame, 9);
	std::vector<cv::Scalar>& skinColorFilterRange = this->calibration.getSkinColorFilterRange();
	cv::inRange(blurredFrame, skinColorFilterRange[0], skinColorFilterRange[1], mask);
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 3);
	cv::cvtColor(frame, frame, cv::COLOR_YCrCb2BGR);
	imshow("Mask", mask);
	
	int squareSize = frame.rows / 2.2;
	int squareThickness = 2;
	cv::Rect alertRectangle(0, (frame.rows - squareSize) / 2, squareSize + 2 * squareThickness, squareSize + 2 * squareThickness);
	cv::rectangle(frame, alertRectangle, cv::Scalar(0, 255, 255), squareThickness, cv::LINE_4);
	cv::Rect roiRect(0, (frame.rows - squareSize) / 2 + squareThickness, squareSize, squareSize);

	cv::Mat roiMask = mask(roiRect);
	cv::resize(roiMask, roiMask, cv::Size(16, 16));
	roiMask = roiMask.reshape(0, 1);
	roiMask.convertTo(roiMask, CV_32FC1);
	cv::Mat prediction;
	ann->predict(roiMask, prediction);
	double min;
	double max;
	cv::Point minLoc;
	cv::Point maxLoc;
	prediction = prediction > 175;
	cv::minMaxLoc(prediction, &min, &max, &minLoc, &maxLoc);
	switch (maxLoc.x) {
	case 0: std::cout << "Paper" << std::endl; break;
	case 1: std::cout << "Rock" << std::endl; break;
	case 2: std::cout << "Scissors" << std::endl; break;
	}
}