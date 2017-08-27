#include "MidRPSGameAI.h"

MidRPSGameAI::MidRPSGameAI()
{
}

Gesture MidRPSGameAI::getAIResponse(Gesture userGesture) {
	switch (userGesture) {
	case Gesture::PAPER: return Gesture::PAPER;
	case Gesture::ROCK: return Gesture::PAPER;
	case Gesture::SCISSORS: return Gesture::SCISSORS;
	default: return Gesture::UNKNOWN;
	}
}

MidRPSGameAI::~MidRPSGameAI()
{
}
