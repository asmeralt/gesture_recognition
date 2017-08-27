#pragma once
#include "Gesture.h"

class RPSGameAI
{
public:
	virtual Gesture getAIResponse(Gesture userGesture) = 0;
};

