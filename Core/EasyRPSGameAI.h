#pragma once
#include "RPSGameAI.h"

class EasyRPSGameAI :
	public RPSGameAI
{
public:
	EasyRPSGameAI();

	virtual Gesture getAIResponse(Gesture userGesture);

	~EasyRPSGameAI();
};

