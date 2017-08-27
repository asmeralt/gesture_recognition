#pragma once
#include "RPSGameAI.h"

class MidRPSGameAI :
	public RPSGameAI
{
public:
	MidRPSGameAI();

	virtual Gesture getAIResponse(Gesture userGesture);

	~MidRPSGameAI();
};

