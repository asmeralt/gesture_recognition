#include "RawSampleFormatter.h"
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>

RawSampleFormatter::RawSampleFormatter(ClassFormatter* classFormatter, const cv::Size size, const int maxClassId): SampleFormatter(classFormatter), size(size), maxClassId(maxClassId)
{
}

cv::Ptr<cv::ml::TrainData> RawSampleFormatter::format(std::vector<Sample> samples) {
	cv::Mat samplesMat((int)samples.size(), this->size.height * this->size.width, CV_32FC1);
	cv::Mat classesMat((int)samples.size(), this->classFormatter->getRequiredColumns(), this->classFormatter->getRequiredType());
	size_t currentRow = 0;
	for (std::vector<Sample>::iterator it = samples.begin(); it != samples.end(); ++it) {
		cv::Mat sampleImage = it->image;
		// Resize image
		cv::resize(sampleImage, sampleImage, this->size);
		// Linearize image and store it in a row
		sampleImage.reshape(0, 1).row(0).copyTo(samplesMat.row(currentRow));
		//Processing image class
		this->classFormatter->format(it->classId).row(0).copyTo(classesMat.row(currentRow));
		++currentRow;
	}
	return cv::ml::TrainData::create(samplesMat, cv::ml::ROW_SAMPLE, classesMat);
}

cv::Mat RawSampleFormatter::format(cv::Mat& sample) {
	cv::Mat sampleMat(1, this->size.height * this->size.width, CV_32FC1);
	cv::Mat rowImage = sample;
	cv::resize(rowImage, rowImage, this->size);
	rowImage.reshape(0, 1).row(0).copyTo(sampleMat.row(0));
	return sampleMat;
}

RawSampleFormatter::~RawSampleFormatter()
{
	SampleFormatter::~SampleFormatter();
}
