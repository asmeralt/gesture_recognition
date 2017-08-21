#include <iostream>
#include <fstream>
#include <list>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\ml.hpp>

#define SAMPLE_SIZE 16
#define THRESHOLD 175
#define MAX_TRAIN_ITER 100
#define PRECISION 0.0001

struct DataSampleInfo {
	int classId;
	std::string file;

	DataSampleInfo(int classId, std::string file) :classId(classId), file(file) {

	}
};

uchar* ind2vec(int ind, int size) {
	uchar* res = new uchar[size];
	for (int i = 0; i < size; ++i) {
		res[i] = 0;
	}
	res[ind] = 1;
	return res;
}

int readSamples(cv::Mat& samples, cv::Mat& classes, std::string batchFileName) {
	std::ifstream batchFile;
	batchFile.open(batchFileName);

	int classId;
	int maxClassId = INT32_MIN;
	std::string file;
	std::list<DataSampleInfo> sampleInfos;
	while (batchFile >> classId >> file) {
		sampleInfos.push_back(DataSampleInfo(classId, file));
		if (maxClassId < classId) {
			maxClassId = classId;
		}
	}
	batchFile.close();
	samples = cv::Mat(sampleInfos.size(), SAMPLE_SIZE * SAMPLE_SIZE, CV_32FC1);
	classes = cv::Mat(sampleInfos.size(), maxClassId, CV_32FC1, cv::Scalar(0));
	int currentRow = 0;
	for (std::list<DataSampleInfo>::iterator it = sampleInfos.begin(); it != sampleInfos.end(); ++it) {
		//Processing image sample
		cv::Mat im = cv::imread(it->file, cv::IMREAD_GRAYSCALE);
		cv::resize(im, im, cv::Size(SAMPLE_SIZE, SAMPLE_SIZE));
		im = im.reshape(0, 1); // linearize image
		im.row(0).copyTo(samples.row(currentRow));
		//Processing image class
		classes.at<float>(currentRow, it->classId - 1) = UCHAR_MAX;
		currentRow++;
	}
	return maxClassId;
}

void thresholdPredictions(cv::Mat& predictions) {
	for (int i = 0; i < predictions.rows; ++i) {
		float* row = predictions.ptr<float>(i);
		for (int j = 0; j < predictions.cols; ++j) {
			if (row[j] < THRESHOLD) {
				row[j] = 0;
			}
			else {
				row[j] = UCHAR_MAX;
			}
		}
	}
}

int loadSamples(cv::Mat& trainSamples, cv::Mat& trainClasses, cv::Mat& testSamples, cv::Mat& testClasses, std::string trainBatchFileName, std::string testBatchFileName) {
	int trainClassesCount = readSamples(trainSamples, trainClasses, trainBatchFileName);
	int testClassesCount = readSamples(testSamples, testClasses, testBatchFileName);
	return (trainClassesCount >= testClassesCount)? trainClassesCount : throw std::exception("Amount of classes in testing set is bigger than in training set.");
}

cv::Ptr<cv::ml::ANN_MLP> buildANN(int classesCount) {
	cv::Ptr<cv::ml::ANN_MLP> ann = cv::ml::ANN_MLP::create();
	cv::Mat_<int> layers(3, 1);
	layers(0) = SAMPLE_SIZE * SAMPLE_SIZE;     // input
	layers(1) = 20;  // hidden
	layers(2) = classesCount;      // output, 1 pin per class.
	ann->setLayerSizes(layers);
	ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	ann->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, MAX_TRAIN_ITER, PRECISION));
	ann->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, PRECISION);
	return ann;
}

void trainANN(cv::Ptr<cv::ml::ANN_MLP> ann, cv::Mat& trainSamples, cv::Mat& trainClasses) {
	cv::Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(trainSamples, cv::ml::ROW_SAMPLE, trainClasses);
	ann->train(trainSamples, cv::ml::ROW_SAMPLE, trainClasses);
}

cv::Ptr<cv::ml::ANN_MLP> loadANN(std::string file) {
	return cv::ml::ANN_MLP::load(file);
}

void saveANN(cv::Ptr<cv::ml::ANN_MLP> ann, std::string file) {
	ann->save(file);
}

float testAnn(cv::Ptr<cv::ml::ANN_MLP> ann, cv::Mat& testSamples, cv::Mat& testClasses) {
	cv::Mat predictions;
	ann->predict(testSamples, predictions);
	thresholdPredictions(predictions);
	cv::Mat error = predictions - testClasses;

	int wrongPredictionsCount = 0;
	for (int i = 0; i < error.rows; ++i) {
		float* row = error.ptr<float>(i);
		for (int j = 0; j < error.cols; ++j) {
			if (abs(row[j]) > PRECISION) {
				++wrongPredictionsCount;
				break;
			}
		}
	}
	return 1.0f * wrongPredictionsCount / error.rows;
}

int main(int argc, char** argv) {
	cv::Mat trainSamples;
	cv::Mat trainClasses;
	cv::Mat testSamples;
	cv::Mat testClasses;

	std::clog << "Loading samples: ";
	int classes = loadSamples(trainSamples, trainClasses, testSamples, testClasses, argv[3], argv[4]);
	std::clog << " DONE." << std::endl;

	cv::Ptr<cv::ml::ANN_MLP> ann;
	if (strcmp(argv[1], "train") == 0) {
		std::clog << "Building artificial neural network: ";
		ann = buildANN(classes);
		std::clog << " DONE." << std::endl;
		std::clog << "Training artificial neural network: ";
		trainANN(ann, trainSamples, trainClasses);
		std::clog << " DONE." << std::endl;
		std::clog << "Saving artificial neural network: ";
		saveANN(ann, argv[2]);
		std::clog << " DONE." << std::endl;
	} else if (strcmp(argv[1], "load") == 0) {
		std::clog << "Loading artificial neural network: ";
		ann = loadANN(argv[2]);
		std::clog << " DONE." << std::endl;
	}
	else {
		std::cerr << "Invalid first argument";
		return -1;
	}
	
	std::clog << "Testing artificial neural network: ";
	float error = testAnn(ann, testSamples, testClasses);
	std::clog << " DONE." << std::endl;
	std::cout <<"Error: " << error;
	return 0;
}


#undef PRECISION
#undef SAMPLE_SIZE
#undef MAX_TRAIN_ITER
#undef THRESHOLD 