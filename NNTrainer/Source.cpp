#include "ANNTrainer.h"
#include "SampleLoader.h"
#include "RawSampleFormatter.h"
#include "PropsSampleFormatter.h"
#include "ClassResponseTester.h"
#include "VecClassFormatter.h"

#define ANN_LAYERS 3
#define RAW_ANN_HIDDEN_LAYER_NEURONS 20
#define PROPS_ANN_HIDDEN_LAYER_NEURONS 3
#define SAMPLE_SIZE 16
#define MAX_TRAIN_ITER 100
#define PRECISION 0.0001
#define TEST_PRECISION 0.1
#define CLASS_THRESHOLD 200

void loadSamples(std::string trainBatchFileName, std::string testBatchFileName, std::vector<Sample>& trainSamples, std::vector<Sample>& testSamples, int& maxClassId) {
	SampleLoader loader;
	int maxTrainClassId;
	int maxTestClassId;
	loader.load(trainBatchFileName, trainSamples, maxTrainClassId);
	loader.load(testBatchFileName, testSamples, maxTestClassId);

	maxClassId = (maxTrainClassId > maxTestClassId)? maxTrainClassId : maxTestClassId;
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

void setUpRawImageANN(int maxClassId, SampleFormatter*& formatter, int*& neurons) {
	formatter = new RawSampleFormatter(new VecClassFormatter(maxClassId), cv::Size(SAMPLE_SIZE, SAMPLE_SIZE), maxClassId);
	neurons = new int[3];
	neurons[0] = SAMPLE_SIZE * SAMPLE_SIZE;
	neurons[1] = RAW_ANN_HIDDEN_LAYER_NEURONS;
	neurons[2] = maxClassId;
}

void setUpSolidityPerimeterANN(int maxClassId, SampleFormatter*& formatter, int*& neurons) {
	formatter = new PropsSampleFormatter(new VecClassFormatter(maxClassId), maxClassId);
	neurons = new int[3];
	neurons[0] = 2;
	neurons[1] = PROPS_ANN_HIDDEN_LAYER_NEURONS;
	neurons[2] = maxClassId;
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
	
	int* neurons = NULL;
	SampleFormatter* formatter = NULL;
	setUpSolidityPerimeterANN(maxClassId, formatter, neurons);
	std::clog << "Formatting samples: ";
	cv::Ptr<cv::ml::TrainData> trainData = formatter->format(trainSamples);
	cv::Ptr<cv::ml::TrainData> testData = formatter->format(testSamples);
	std::clog << " DONE." << std::endl;
	ANNTrainer annTrainer(ANN_LAYERS, neurons, MAX_TRAIN_ITER, PRECISION);
	
	if (mode == "train") {
		std::clog << "Building artificial neural network: ";
		annTrainer.build();
		std::clog << " DONE." << std::endl;
		std::clog << "Training artificial neural network: ";
		annTrainer.train(trainData);
		std::clog << " DONE." << std::endl;
		std::clog << "Saving artificial neural network: ";
		annTrainer.save(annFileName);
		std::clog << " DONE." << std::endl;
	} else if (mode == "load") {
		std::clog << "Loading artificial neural network: ";
		annTrainer.load(annFileName);
		std::clog << " DONE." << std::endl;
	}
	else {
		std::cerr << "Invalid first argument";
		return -1;
	}

	ClassResponseTester annTester(annTrainer.getModel(), CLASS_THRESHOLD);
	
	std::clog << "Testing artificial neural network: ";
	float error = annTester.test(testData, TEST_PRECISION);
	std::clog << " DONE." << std::endl;
	std::cout <<"Error: " << error;
	return 0;
}


#undef CLASS_THRESHOLD
#undef TEST_PRECISION
#undef PRECISION
#undef MAX_TRAIN_ITER
#undef SAMPLE_SIZE
#undef PROPS_ANN_HIDDEN_LAYER_NEURONS
#undef RAW_ANN_HIDDEN_LAYER_NEURONS
#undef ANN_LAYERS