#pragma once
#include <opencv2\ml.hpp>

class Trainer
{
public:

	virtual void build() = 0;

	virtual void load(std::string file) = 0;

	virtual void save(std::string file) = 0;

	virtual void train(cv::Ptr<cv::ml::TrainData> data) = 0;

	virtual cv::Ptr<cv::ml::StatModel> getModel() = 0;

};

