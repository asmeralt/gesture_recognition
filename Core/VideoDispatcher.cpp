#include "VideoDispatcher.h"
#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>
#include <opencv2\imgproc.hpp>

// public
VideoDispatcher::VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, int gameDurationTimeSec, std::vector<GestureRecognizer*>& recognizers, std::vector<GestureRecognizer*>& gameRecognizers, std::vector<RPSGameAI*> gameAIs):
	windowName(windowName),
	frameCaptureDelayMillis(frameCaptureDelayMillis),
	state(VD_CALIBRATION),
	recognizers(recognizers),
	gameRecognizers(gameRecognizers),
	gameAIs(gameAIs)
{
	currentRecognizerIdx = 0;
	currentGameRecognizerIdx = 0;
	this->calibration = new CalibrationStateExecutor(new SkinCalibrator(), 0.3f);
	this->recognition = new RecognitionStateExecutor(new ColorMasker(), new GestureRoiFinder(), this->recognizers[currentRecognizerIdx]);
	this->game = new GameStateExecutor(gameDurationTimeSec, new ColorMasker(), new GestureRoiFinder(), this->gameRecognizers[currentGameRecognizerIdx], gameAIs);
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
	if (state = VD_RECOGNITION) {
		currentRecognizerIdx = (currentRecognizerIdx + 1) % this->recognizers.size();
		recognition->setRecognizer(this->recognizers[currentRecognizerIdx]);
	}
	else if (state = VD_GAME) {
		currentGameRecognizerIdx = (currentGameRecognizerIdx + 1) % this->gameRecognizers.size();
		game->setRecognizer(this->gameRecognizers[currentGameRecognizerIdx]);
	}
}


void VideoDispatcher::deleteRecognizers(std::vector<GestureRecognizer*>& recognizers, size_t currentRecognizerIdx) {
	for (int i = 0; i < recognizers.size(); ++i) {
		if (i != currentRecognizerIdx) {
			delete recognizers[i];
		}
	}
}

VideoDispatcher::~VideoDispatcher()
{
	delete calibration;
	delete recognition;
	delete game;
	deleteRecognizers(recognizers, currentRecognizerIdx);
	deleteRecognizers(gameRecognizers, currentGameRecognizerIdx);
}