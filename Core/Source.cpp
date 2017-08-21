#include <iostream>
#include <opencv2\ml.hpp>
#include "SkinCalibrator.h"
#include "CalibrationStateExecutor.h"
#include "VideoDispatcher.h"

// UI names consts
const std::string windowName = "Gesture recognition";

// Controls for a frame rate
const int frameCaptureDelayMillis = 40;  // approx 25 frames per second

int main(int argc, char** argv) {
	VideoDispatcher dispatcher ("Gesture detector", frameCaptureDelayMillis, CalibrationStateExecutor(SkinCalibrator(), 0.3f), cv::ml::ANN_MLP::load("..\\data\\nn\\networks\\ann.yml"));

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
