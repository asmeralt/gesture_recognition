#pragma once
#include <string>
#include <opencv2\core.hpp>
#include <opencv2\ml.hpp>
#include "VideoDispatcherState.h"
#include "SkinCalibrator.h"
#include "CalibrationStateExecutor.h"
#include "RecognitionStateExecutor.h"
#include "GameStateExecutor.h"
#include "GestureRecognizer.h"

class VideoDispatcher
{
private:
	std::string windowName;
	int frameCaptureDelayMillis;
	size_t currentRecognizerIdx;

	/* State machine */
	VideoDispatcherState state;
	CalibrationStateExecutor* calibration;
	RecognitionStateExecutor* recognition;
	GameStateExecutor* game;

	/* Recognizers */
	std::vector<GestureRecognizer*> recognizers;

public:
	VideoDispatcher(std::string windowName, int frameCaptureDelayMillis, int gameDurationTimeSec, std::vector<GestureRecognizer*>& recognizers);

	void run();

	~VideoDispatcher();
private:
	void switchToCalibration();

	void switchToRecognition(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange);

	void switchToGame(cv::Mat& frame, std::vector<cv::Scalar>& skinColorRange);

	void switchToNextRecognizer();
};

