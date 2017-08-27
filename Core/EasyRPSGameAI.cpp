#include "EasyRPSGameAI.h"

EasyRPSGameAI::EasyRPSGameAI()
{
}

Gesture EasyRPSGameAI::getAIResponse(Gesture userGesture) {
	switch (userGesture) {
	case Gesture::PAPER: return Gesture::ROCK;
	case Gesture::ROCK: return Gesture::SCISSORS;
	case Gesture::SCISSORS: return Gesture::PAPER;
	default: return Gesture::UNKNOWN;
	}
}

EasyRPSGameAI::~EasyRPSGameAI()
{
}
