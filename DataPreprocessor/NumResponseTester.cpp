#include "NumResponseTester.h"

NumResponseTester::NumResponseTester(cv::Ptr<cv::ml::StatModel> sm): Tester(sm)
{
}

float NumResponseTester::test(cv::Ptr<cv::ml::TrainData> data, const double precision) {
	cv::Mat predictions;
	sm->predict(data->getSamples(), predictions);
	cv::Mat errors = predictions - data->getResponses();

	int wrongPredictionsCount = 0;
	for (int i = 0; i < errors.rows; ++i) {
		float* errorRow = errors.ptr<float>(i);
		float* predictionRow = predictions.ptr<float>(i);
		double errorNorm = calcEuclideanNorm(errorRow, errors.cols);
		double predictionRowNorm = calcEuclideanNorm(predictionRow, predictions.cols);
		if (errorNorm / predictionRowNorm > precision) {
			++wrongPredictionsCount;
		}
	}
	return 1.0f * wrongPredictionsCount / errors.rows;
}

double NumResponseTester::calcEuclideanNorm(float* row, int length) {
	double norm = 0;
	for (int i = 0; i < length; ++i) {
		norm += row[i] * row[i];
	}
	return std::sqrt(norm);
}

NumResponseTester::~NumResponseTester()
{
}
