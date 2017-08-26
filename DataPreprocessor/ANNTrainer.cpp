#include "ANNTrainer.h"

ANNTrainer::ANNTrainer(int layersCount, int* neurons, int maxTrainIterCount, double precision): layersCount(layersCount), 
neurons(neurons), 
maxTrainIterCount(maxTrainIterCount), 
precision(precision)
{
}

void ANNTrainer::build() {
	this->ann = cv::ml::ANN_MLP::create();
	cv::Mat_<int> layers(this->layersCount, 1);
	for (int i = 0; i < this->layersCount; ++i) {
		layers(i) = this->neurons[i];
	}
	ann->setLayerSizes(layers);
	ann->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 1.0, 1.0);
	ann->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS,this->maxTrainIterCount, this->precision));
	ann->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, this->precision);
}

void ANNTrainer::load(std::string file) {
	this->ann = cv::ml::ANN_MLP::load(file);
}

void ANNTrainer::save(std::string file) {
	this->ann->save(file);
}

void ANNTrainer::train(cv::Ptr<cv::ml::TrainData> data) {
	this->ann->train(data);
}

cv::Ptr<cv::ml::StatModel> ANNTrainer::getModel() {
	return ann;
}

//void ANNTrainer::filterClassPredictions(cv::Mat& predictions) {
//	for (int i = 0; i < predictions.rows; ++i) {
//		float* row = predictions.ptr<float>(i);
//		for (int j = 0; j < predictions.cols; ++j) {
//			row[j] = (row[j] > 200) ? 255: 0;
//		}
//	}
//}

ANNTrainer::~ANNTrainer()
{
}