#include "StatModelRecognizer.h"

StatModelRecognizer::StatModelRecognizer(cv::Ptr<cv::ml::StatModel> model, ImageFormatter* formatter): model(model), formatter(formatter)
{
}

cv::Mat StatModelRecognizer::predict(cv::Mat& image) {
	cv::Mat prediction;
	this->model.get()->predict(formatter->format(image), prediction);
	return prediction;
}

StatModelRecognizer::~StatModelRecognizer()
{
	this->model.release();
}
