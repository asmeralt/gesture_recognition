#include "GameStateExecutor.h"
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

GameStateExecutor::GameStateExecutor(int gameDurationSec, ColorMasker* colorMasker, GestureRoiFinder* roiFinder, GestureRecognizer* recognizer, std::vector<RPSGameAI*>& gameAIs) :
	RecognitionStateExecutor(colorMasker, roiFinder, recognizer),
	gameDurationSec(gameDurationSec),
	gameAIs(gameAIs),
	isStarted(false),
	startTicks(0),
	currentAIIdx(0)
{
}

void GameStateExecutor::execute(cv::Mat& frame) {
	if (this->isStarted) {
		cv::destroyWindow("Game result");
		cv::Mat mask = maskFrame(frame);
		cv::Rect roiRect = findRoi(mask);
		plotGestureRoi(frame, roiRect);
		int timePassedSec = timePassed();
		if (isTimeToEnd(timePassedSec)) {
			cv::Mat roiMask = mask(roiRect);
			Gesture userGesture = recognizeGesture(roiMask);
			Gesture aiGesture = this->gameAIs[currentAIIdx]->getAIResponse(userGesture);
			GameResult gameResult = findGameResult(userGesture, aiGesture);
			processResult(frame, gameResult, userGesture, aiGesture);
			this->isStarted = false;
		}
		else {
			plotTimeLeft(frame, timePassedSec);
		}
	}
	else {
		cv::imshow("Game result", this->gameResultFrame);
	}
}

void GameStateExecutor::start() {
	this->isStarted = true;
	this->startTicks = (double)cv::getTickCount();
}

void GameStateExecutor::nextAI() {
	if (currentAIIdx < gameAIs.size() - 1) {
		++currentAIIdx;
	}
}

void GameStateExecutor::previousAI() {
	if (currentAIIdx > 0) {
		--currentAIIdx;
	}
}

int GameStateExecutor::timePassed() {
	return ((double)cv::getTickCount() - this->startTicks) / cv::getTickFrequency();
}

bool GameStateExecutor::isTimeToEnd(int timePassed) {
	return timePassed > this->gameDurationSec;
}

void GameStateExecutor::plotTimeLeft(cv::Mat& frame, int timePassedSec) {
	uint timeLeft = this->gameDurationSec - timePassedSec + 1;
	cv::putText(frame, cv::format("%i", timeLeft), cv::Point(frame.cols / 2 - 50, frame.rows / 2 - 20), cv::FONT_HERSHEY_PLAIN, 10, cv::Scalar(220, 75, 75), 5);
}

bool GameStateExecutor::isAcceptable(Gesture gesture) {
	return gesture < 3;
}

void GameStateExecutor::processResult(cv::Mat& frame, GameResult gameResult, Gesture userGesture, Gesture aiGesture) {
	std::string userGestureName = getGestureName(userGesture);
	std::string aiGestureName = getGestureName(aiGesture);
	std::string resultName = getGameResultName(gameResult);
	cv::Scalar resultNameColor = getGameResultColor(gameResult);
	this->gameResultFrame = frame.clone();
	cv::putText(gameResultFrame, resultName, cv::Point(gameResultFrame.cols / 2 - 100, gameResultFrame.rows / 2 - 150), cv::FONT_HERSHEY_PLAIN, 5, resultNameColor, 3);
	cv::putText(gameResultFrame, "You: " + userGestureName, cv::Point(gameResultFrame.cols / 2 - 100, gameResultFrame.rows / 2 - 100), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 1);
	cv::putText(gameResultFrame, "Oponent: " + aiGestureName, cv::Point(gameResultFrame.cols / 2 - 100, gameResultFrame.rows / 2 - 70), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(255, 255, 255), 1);
	plotAIInfo(gameResultFrame);
}

std::string GameStateExecutor::getGameResultName(GameResult gameResult) {
	switch (gameResult) {
	case GameResult::DRAW: return"Draw";
	case GameResult::LOOSE: return "Loose";
	case GameResult::WICTORY: return"Wictory";
	default: return "Fail";
	}
}

cv::Scalar GameStateExecutor::getGameResultColor(GameResult gameResult) {
	switch (gameResult) {
	case GameResult::DRAW: return cv::Scalar(220, 75, 75); break;
	case GameResult::LOOSE: return cv::Scalar(75, 75, 220); break;
	case GameResult::WICTORY: return cv::Scalar(75, 220, 75); break;
	default: return cv::Scalar(150, 150, 50); break;
	}
}

GameResult GameStateExecutor::findGameResult(Gesture userGesture, Gesture aiGesture) {
	if (isAcceptable(userGesture)) {
		int diff = (userGesture - aiGesture + 3) % 3;
		switch (diff) {
		case 0: return GameResult::DRAW;
		case 1: return GameResult::LOOSE;
		case 2: return GameResult::WICTORY;
		}
	}
	else {
		return GameResult::FAIL;
	}
}

void GameStateExecutor::plotAIInfo(cv::Mat& frame) {
	cv::putText(frame, typeid(*(this->gameAIs[currentAIIdx])).name(), cv::Point(frame.cols - 200, 20), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(255, 255, 255));
}

GameStateExecutor::~GameStateExecutor()
{
}
