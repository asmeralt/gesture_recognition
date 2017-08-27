#include "VideoDispatcher.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>
#include "EasyRPSGameAI.h"
#include "MidRPSGameAI.h"
#include "RandomRPSGameAI.h"
#include "HardRPSGameAI.h"

// public
VideoDispatcher::VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, int gameDurationTimeSec, std::vector<GestureRecognizer*>& recognizers):
	windowName(windowName),
	frameCaptureDelayMillis(frameCaptureDelayMillis),
	state(VD_CALIBRATION),
	recognizers(recognizers)
{
	currentRecognizerIdx = 0;
	this->calibration = new CalibrationStateExecutor(new SkinCalibrator(), 0.3f);
	this->recognition = new RecognitionStateExecutor(new ColorMasker(), new GestureRoiFinder(), this->recognizers[currentRecognizerIdx]);
	std::vector<RPSGameAI*> gameAIs;
	gameAIs.push_back(new EasyRPSGameAI());
	gameAIs.push_back(new MidRPSGameAI());
	gameAIs.push_back(new RandomRPSGameAI());
	gameAIs.push_back(new HardRPSGameAI());
	this->game = new GameStateExecutor(gameDurationTimeSec, new ColorMasker(), new GestureRoiFinder(), this->recognizers[currentRecognizerIdx], gameAIs);
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

	/* !!!DONT DO THIS AT HOME!!! */
	/* Wrong State machine pattern implementation */ 
	/* Appropriate implementation will dramatically increase code complexity */
	while (state != VD_EXIT) {
		webcam >> frame; // reading webcam
		if (frame.empty() || !webcam.isOpened()) { // check if webcam stream is not emtpy or closed
			throw std::exception("Can not read a video stream from your webcam");
		}

		switch (state) {
		case VD_CALIBRATION: calibration->execute(frame); break;
		case VD_RECOGNITION: recognition->execute(frame); break;
		case VD_GAME: game->execute(frame); break;
		case VD_EXIT: return;
		default: throw std::exception("Ooops, given dispatcher state %i is not implemented yet");
		}

		cv::imshow(windowName, frame); // display resulted frame

		// Break "while loop" when Esc is pressed, start calibration when 'c' is pressed, begin recognition when 'r' is pressed
		switch (cv::waitKey(frameCaptureDelayMillis)) {
		case 27: state = VD_EXIT; break; // Esc key code is 27
		case 'c': switchToCalibration(); break;
		case 'r': switchToRecognition(frame, calibration->getSkinColorRange()); break;
		case 'g': switchToGame(frame, calibration->getSkinColorRange()); break;
		case '+': game->nextAI(); break;
		case '-': game->previousAI(); break;
		case 'n': switchToNextRecognizer(); break;
		default: break;
		}
	}
}

void VideoDispatcher::switchToCalibration() {
	state = VD_CALIBRATION;
}

void VideoDispatcher::switchToRecognition(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange) {
	state = VD_RECOGNITION;
	recognition->initExecutor(frame, skinColorRange);
}

void VideoDispatcher::switchToGame(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange) {
	state = VD_GAME;
	game->initExecutor(frame, skinColorRange);
	game->start();
}

void VideoDispatcher::switchToNextRecognizer() {
	currentRecognizerIdx = (currentRecognizerIdx + 1) % this->recognizers.size();
	recognition->setRecognizer(this->recognizers[currentRecognizerIdx]);
}