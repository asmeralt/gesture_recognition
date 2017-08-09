#include "VideoDispatcher.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>

VideoDispatcher::VideoDispatcher(std::string windowName, int frameCaptureDelayMillis):windowName(windowName), frameCaptureDelayMillis(frameCaptureDelayMillis) {
	this->state = VD_CALIBRATION;
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
		case VD_CALIBRATION: calibration(frame); break;
		case VD_RECOGNITION: recognition(frame); break;
		case VD_EXIT: return;
		default: throw std::exception("Ooops, given dispatcher state %i is not implemented yet");
		}

		cv::imshow(windowName, frame); // display resulted frame
	
		// Break while loop when Esc is pressed
		if (cv::waitKey(frameCaptureDelayMillis) == 27) {
			state = VD_EXIT;
		}
	}
}

void VideoDispatcher::calibration(cv::Mat& frame) {
	
}

void VideoDispatcher::recognition(cv::Mat& frame) {

}

VideoDispatcher::~VideoDispatcher()
{
}
