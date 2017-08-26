#include "SampleFormatter.h"

SampleFormatter::SampleFormatter(ImageFormatter* imageFormatter, ClassFormatter* classFormatter): imageFormatter(imageFormatter), classFormatter(classFormatter)
{
}

cv::Ptr<cv::ml::TrainData> SampleFormatter::format(std::vector<Sample>& samples) {
	cv::Mat data(samples.size(), imageFormatter->getRequiredColumns(), imageFormatter->getRequiredType());
	cv::Mat classes(samples.size(), classFormatter->getRequiredColumns(), classFormatter->getRequiredType());
	size_t currentRow = 0;
	for (std::vector<Sample>::iterator it = samples.begin(); it != samples.end(); ++it) {
		imageFormatter->format(it->image).copyTo(data.row(currentRow));
		classFormatter->format(it->classId).copyTo(classes.row(currentRow));
		++currentRow;
	}
	return cv::ml::TrainData::create(data, cv::ml::ROW_SAMPLE, classes);
}

SampleFormatter::~SampleFormatter()
{
}