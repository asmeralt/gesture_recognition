#include "NBCTrainer.h"

NBCTrainer::NBCTrainer()
{
}

void NBCTrainer::build() {
	this->nbc = cv::ml::NormalBayesClassifier::create();
}

void NBCTrainer::load(std::string file) {
	this->nbc = cv::ml::NormalBayesClassifier::load(file);
}

void NBCTrainer::save(std::string file) {
	this->nbc->save(file);
}

void NBCTrainer::train(cv::Ptr<cv::ml::TrainData> data) {
	this->nbc->train(data);
}

cv::Ptr<cv::ml::StatModel> NBCTrainer::getModel() {
	return nbc;
}

//float NBCTrainer::test(cv::Ptr<cv::ml::TrainData> data, const double precision) {
//	cv::Mat predictions;
//	cv::Mat probabilities;
//	nbc->predictProb(data->getSamples(), predictions, probabilities);
//	cv::Mat error = predictions - data->getResponses();
//
//	int wrongPredictionsCount = 0;
//	for (int i = 0; i < error.rows; ++i) {
//		float* row = error.ptr<float>(i);
//		for (int j = 0; j < error.cols; ++j) {
//			if (abs(row[j]) > precision) {
//				++wrongPredictionsCount;
//				break;
//			}
//		}
//	}
//	return 1.0f * wrongPredictionsCount / error.rows;
//}

NBCTrainer::~NBCTrainer()
{
}
