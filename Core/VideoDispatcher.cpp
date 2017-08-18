#include "VideoDispatcher.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>

// public
VideoDispatcher::VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, CalibrationStateExecutor& calibrationStateExecutor) :
	windowName(windowName),
	frameCaptureDelayMillis(frameCaptureDelayMillis),
	state(VD_CALIBRATION),
	calibration(calibrationStateExecutor)
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
		case VD_CALIBRATION: calibration.execute(frame); recognition(frame); break;
		case VD_RECOGNITION: recognition(frame); break;
		case VD_EXIT: return;
		default: throw std::exception("Ooops, given dispatcher state %i is not implemented yet");
		}

		cv::imshow(windowName, frame); // display resulted frame

		// Break "while loop" when Esc is pressed, start calibration when 'c' is pressed, begin recognition when 'r' is pressed
		switch (cv::waitKey(frameCaptureDelayMillis)) {
		case 27: state = VD_EXIT; break; // Esc key code is 27
		case 'c': state = VD_CALIBRATION; save = false; break;
		case 'r': state = VD_RECOGNITION; save = true; break;
		default: break;
		}
	}
}

// private
void VideoDispatcher::recognition(cv::Mat& frame) {
	cv::Mat mask;
	cv::Mat blurredFrame;
	cv::cvtColor(frame, frame, cv::COLOR_BGR2YCrCb);
	cv::medianBlur(frame, blurredFrame, 3);
	std::vector<cv::Scalar>& skinColorFilterRange = this->calibration.getSkinColorFilterRange();
	cv::inRange(blurredFrame, skinColorFilterRange[0], skinColorFilterRange[1], mask);
	cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)), cv::Point(-1, -1), 3);
	//cv::morphologyEx(mask, mask, cv::MORPH_OPEN, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 3);
	//cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15)));
	cv::cvtColor(frame, frame, cv::COLOR_YCrCb2BGR);
	imshow("Mask", mask);
	if (this->save) {
		int squareSize = frame.rows / 1.8;
		int squareThickness = 2;
		cv::Rect alertRectangle(0, (frame.rows - squareSize) / 2, squareSize + 2 * squareThickness, squareSize + 2 * squareThickness);
		cv::rectangle(frame, alertRectangle, cv::Scalar(0, 255, 255), squareThickness, cv::LINE_4);
		cv::Rect roiRect(squareThickness, (frame.rows - squareSize) / 2 + squareThickness, squareSize, squareSize);

		cv::Mat roi = blurredFrame(roiRect);
		cv::Mat roiMask = mask(roiRect);
		cv::cvtColor(roi, roi, cv::COLOR_BGR2GRAY);
		cv::Mat example;
		roi.copyTo(example, roiMask);
		cv::resize(example, example, cv::Size(32, 32));
		bool result = cv::imwrite(cv::format("..\\data\\scissors\\example_%i.bmp", exampleId), example);
		exampleId++;
	}
}