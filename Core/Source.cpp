#include <iostream>
#include <opencv2\ml.hpp>
#include "VideoDispatcher.h"
#include "ANNRawRecognizer.h"
#include "ANNPropsRecognizer.h"
#include "NBCPropsRecognizer.h"
#include "EasyRPSGameAI.h"
#include "MidRPSGameAI.h"
#include "RandomRPSGameAI.h"
#include "HardRPSGameAI.h"
#include "FeatureProps.h"

// UI names consts
const int gameDurationTimeSec = 3;
const std::string windowName = "Gesture recognition";

// Controls for a frame rate
const int frameCaptureDelayMillis = 40;  // approx 25 frames per second

PropsImageFormatter* buildSolidityPerimeterPropsImageFormatter() {
	std::vector<float(*)(std::vector<cv::Point>&)> props;
	props.push_back(FeatureProps::calcSolidity);
	props.push_back(FeatureProps::calcContourPerimeter);
	return new PropsImageFormatter(FeatureProps::findMaxContour, props);

}

std::vector<GestureRecognizer*> loadRecognizers(char* annRawFile, char* annPropsFile, char* nbcPropsFile) {
	std::vector<GestureRecognizer*> recognizers(3);
	recognizers[0] = new ANNRawRecognizer(cv::ml::ANN_MLP::load(annRawFile), new RawImageFormatter(cv::Size(16, 16)));
	recognizers[1] = new ANNPropsRecognizer(cv::ml::ANN_MLP::load(annPropsFile), buildSolidityPerimeterPropsImageFormatter());
	recognizers[2] = new NBCPropsRecognizer(cv::ml::NormalBayesClassifier::load(nbcPropsFile), buildSolidityPerimeterPropsImageFormatter());
	return recognizers;
}

std::vector<RPSGameAI*> loadGameAIs() {
	std::vector<RPSGameAI*> gameAIs;
	gameAIs.push_back(new EasyRPSGameAI());
	gameAIs.push_back(new MidRPSGameAI());
	gameAIs.push_back(new RandomRPSGameAI());
	gameAIs.push_back(new HardRPSGameAI());
	return gameAIs;
}

int main(int argc, char** argv) {
	std::vector<GestureRecognizer*> recognizers = loadRecognizers("..\\data\\recognizers\\annRaw.yml","..\\data\\recognizers\\annProps.yml","..\\data\\recognizers\\nbcProps.yml");
	std::vector<GestureRecognizer*> gameRecognizers = loadRecognizers("..\\data\\recognizers\\annRaw.yml", "..\\data\\recognizers\\annProps.yml", "..\\data\\recognizers\\nbcProps.yml");
	std::vector<RPSGameAI*> gameAIs = loadGameAIs();
	VideoDispatcher dispatcher ("Gesture detector", frameCaptureDelayMillis, gameDurationTimeSec, recognizers, gameRecognizers, gameAIs);

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
