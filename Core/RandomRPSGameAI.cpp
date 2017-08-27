#include "RandomRPSGameAI.h"
#include <random>

RandomRPSGameAI::RandomRPSGameAI()
{
}

Gesture RandomRPSGameAI::getAIResponse(Gesture userGesture) {
	return static_cast<Gesture>(std::rand() % 3);
}

RandomRPSGameAI::~RandomRPSGameAI()
{
}
