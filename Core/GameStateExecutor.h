#pragma once
#include "RecognitionStateExecutor.h"
#include "GameResult.h"
#include "RPSGameAI.h"

class GameStateExecutor :
	public RecognitionStateExecutor
{
private:
	bool isStarted;
	double startTicks;
	int gameDurationSec;
	cv::Mat gameResultFrame;
	int currentAIIdx;
	std::vector<RPSGameAI*> gameAIs;
public:
	GameStateExecutor(int gameDurationSec, ColorMasker* colorMasker, GestureRoiFinder* roiFinder, GestureRecognizer* recognizer, std::vector<RPSGameAI*>& gameAIs);

	virtual void execute(cv::Mat& frame);

	void start();

	void nextAI();

	void previousAI();

	~GameStateExecutor();
private:

	int timePassed();

	bool isTimeToEnd(int timePassed);

	void plotTimeLeft(cv::Mat& frame, int timePassedSec);

	bool isAcceptable(Gesture gesture);

	void processResult(cv::Mat& frame, GameResult gameResult, Gesture userGesture, Gesture aiGesture);

	std::string getGameResultName(GameResult gameResult);

	cv::Scalar getGameResultColor(GameResult gameResult);

	GameResult findGameResult(Gesture userGesture, Gesture aiGesture);

	void plotAIInfo(cv::Mat& frame);
};

