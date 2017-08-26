#pragma once
#include "Trainer.h"
class NBCTrainer :
	public Trainer
{
private:
	cv::Ptr<cv::ml::NormalBayesClassifier> nbc;

public:
	NBCTrainer();

	virtual void build();

	virtual void load(std::string file);

	virtual void save(std::string file);

	virtual void train(cv::Ptr<cv::ml::TrainData> data);

	virtual cv::Ptr<cv::ml::StatModel> getModel();

	~NBCTrainer();
};

