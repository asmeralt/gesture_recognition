#pragma once
#include "Trainer.h"
class ANNTrainer :
	public Trainer
{
private:
	const int layersCount;
	const int* neurons;
	const int maxTrainIterCount;
	const double precision;
	cv::Ptr<cv::ml::ANN_MLP> ann;
public:
	ANNTrainer(int layersCount, int* neurons, int maxTrainIterCount, double precision);

	virtual void build();

	virtual void load(std::string file);

	virtual void save(std::string file);

	virtual void train(cv::Ptr<cv::ml::TrainData> data);

	virtual cv::Ptr<cv::ml::StatModel> getModel();

	~ANNTrainer();
};

