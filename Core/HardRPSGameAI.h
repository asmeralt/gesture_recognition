#pragma once
#include "RPSGameAI.h"

class HardRPSGameAI :
	public RPSGameAI
{
public:
	HardRPSGameAI();

	virtual Gesture getAIResponse(Gesture userGesture);

	~HardRPSGameAI();
};

