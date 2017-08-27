#pragma once
#include "RPSGameAI.h"

class RandomRPSGameAI :
	public RPSGameAI
{
public:
	RandomRPSGameAI();

	virtual Gesture getAIResponse(Gesture userGesture);

	~RandomRPSGameAI();
};

