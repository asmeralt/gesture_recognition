#include <iostream>
#include "VideoDispatcher.h"

// UI names consts
const std::string windowName = "Gesture recognition";

// Controls for a frame rate
const int frameCaptureDelayMillis = 40;  // approx 25 frames per second

int main(int argc, char** argv) {
	VideoDispatcher dispatcher ("Gesture detector", frameCaptureDelayMillis);

	try {
		dispatcher.run();
	}
	catch (std::exception exc) {
		std::cout << exc.what() << std::endl;
		system("pause");
		return -1;
	}
	return 0;
}
