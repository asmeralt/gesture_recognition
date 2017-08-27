#include "HardRPSGameAI.h"

HardRPSGameAI::HardRPSGameAI()
{
}

Gesture HardRPSGameAI::getAIResponse(Gesture userGesture) {
	switch (userGesture) {
	case Gesture::PAPER: return Gesture::SCISSORS;
	case Gesture::ROCK: return Gesture::PAPER;
	case Gesture::SCISSORS: return Gesture::ROCK;
	default: return Gesture::UNKNOWN;
	}
}

HardRPSGameAI::~HardRPSGameAI()
{
}
