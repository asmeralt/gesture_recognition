#include "ClassResponseTester.h"

ClassResponseTester::ClassResponseTester(cv::Ptr<cv::ml::StatModel> sm, float classThreshold): Tester(sm), classThreshold(classThreshold)
{
}

float ClassResponseTester::test(cv::Ptr<cv::ml::TrainData> data, const double precision) {
	cv::Mat predictions;
	sm->predict(data->getSamples(), predictions);
	thresholdPredictions(predictions);
	cv::Mat errors = predictions - data->getResponses();

	int wrongPredictionsCount = 0;
	for (int i = 0; i < errors.rows; ++i) {
		float* row = errors.ptr<float>(i);
		for (int j = 0; j < predictions.cols; ++j) {
			if (row[j] > 0) {
				++wrongPredictionsCount;
				break;
			}
		}
	}
	return 1.0f * wrongPredictionsCount / errors.rows;
}

void ClassResponseTester::thresholdPredictions(cv::Mat& predictions) {
	for (int i = 0; i < predictions.rows; ++i) {
		float* row = predictions.ptr<float>(i);
		for (int j = 0; j < predictions.cols; ++j) {
			if (row[j] > classThreshold) {
				row[j] = UCHAR_MAX;
			}
			else {
				row[j] = 0;
			}
		}
	}
}

ClassResponseTester::~ClassResponseTester()
{
}
