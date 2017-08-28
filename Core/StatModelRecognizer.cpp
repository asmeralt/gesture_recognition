#include "StatModelRecognizer.h"

StatModelRecognizer::StatModelRecognizer(cv::Ptr<cv::ml::StatModel> model): model(model)
{
}

cv::Mat StatModelRecognizer::predict(cv::Mat& data) {
	cv::Mat prediction;
	this->model.get()->predict(data, prediction);
	return prediction;
}

StatModelRecognizer::~StatModelRecognizer()
{
	this->model.release();
}
