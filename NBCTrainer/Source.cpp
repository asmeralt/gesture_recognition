#include "NBCTrainer.h"
#include "SampleLoader.h"
#include "SampleFormatter.h"
#include "RawImageFormatter.h"
#include "PropsImageFormatter.h"
#include "NumResponseTester.h"
#include "NumClassFormatter.h"

#define SAMPLE_SIZE 16
#define TEST_PRECISION 0.1

void loadSamples(std::string trainBatchFileName, std::string testBatchFileName, std::vector<Sample>& trainSamples, std::vector<Sample>& testSamples, int& maxClassId) {
	SampleLoader loader;
	int maxTrainClassId;
	int maxTestClassId;
	loader.load(trainBatchFileName, trainSamples, maxTrainClassId);
	loader.load(testBatchFileName, testSamples, maxTestClassId);

	maxClassId = (maxTrainClassId > maxTestClassId) ? maxTrainClassId : maxTestClassId;
}

std::string mode;
std::string annFileName;
std::string trainBatchFileName;
std::string testBatchFileName;

bool parseArgs(int argc, char** argv) {
	if (argc != 5) {
		return false;
	}
	mode = argv[1];
	annFileName = argv[2];
	trainBatchFileName = argv[3];
	testBatchFileName = argv[4];
	return true;
}

void setUpRawImageNBC(SampleFormatter*& formatter) {
	formatter = new SampleFormatter(new RawImageFormatter(cv::Size(SAMPLE_SIZE, SAMPLE_SIZE)), new NumClassFormatter(CV_32SC1));
}

void setUpSolidityPerimeterNBC(SampleFormatter*& formatter) {
	formatter = new SampleFormatter(new PropsImageFormatter(), new NumClassFormatter(CV_32SC1));
}

int main(int argc, char** argv) {
	if (!parseArgs(argc, argv)) {
		std::cerr << "Invalid arguments";
		return -1;
	}

	std::vector<Sample> trainSamples;
	std::vector<Sample> testSamples;
	int maxClassId;

	std::clog << "Loading samples: ";
	loadSamples(trainBatchFileName, testBatchFileName, trainSamples, testSamples, maxClassId);
	std::clog << " DONE." << std::endl;

	SampleFormatter* formatter = NULL;
	setUpRawImageNBC(formatter);
	std::clog << "Formatting samples: ";
	cv::Ptr<cv::ml::TrainData> trainData = formatter->format(trainSamples);
	cv::Ptr<cv::ml::TrainData> testData = formatter->format(testSamples);
	std::clog << " DONE." << std::endl;
	NBCTrainer nbcTrainer;

	if (mode == "train") {
		std::clog << "Building naive bayes classifier: ";
		nbcTrainer.build();
		std::clog << " DONE." << std::endl;
		std::clog << "Training naive bayes classifier: ";
		nbcTrainer.train(trainData);
		std::clog << " DONE." << std::endl;
		std::clog << "Saving naive bayes classifier: ";
		nbcTrainer.save(annFileName);
		std::clog << " DONE." << std::endl;
	}
	else if (mode == "load") {
		std::clog << "Loading naive bayes classifier: ";
		nbcTrainer.load(annFileName);
		std::clog << " DONE." << std::endl;
	}
	else {
		std::cerr << "Invalid first argument";
		return -1;
	}

	NumResponseTester nbcTester(nbcTrainer.getModel());

	std::clog << "Testing naive bayes classifier: ";
	float error = nbcTester.test(testData, TEST_PRECISION);
	std::clog << " DONE." << std::endl;
	std::cout << "Error: " << error;
	return 0;
}

#undef TEST_PRECISION
#undef SAMPLE_SIZE