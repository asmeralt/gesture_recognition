#include "PropsSampleFormatter.h"
#include "FeatureProps.h"

PropsSampleFormatter::PropsSampleFormatter(ClassFormatter* classFormatter, int maxClassId):SampleFormatter(classFormatter), maxClassId(maxClassId)
{
}

cv::Ptr<cv::ml::TrainData> PropsSampleFormatter::format(std::vector<Sample> samples) {
	cv::Mat samplesMat((int)samples.size(), 2, CV_32FC1);
	cv::Mat classesMat((int)samples.size(), this->classFormatter->getRequiredColumns(), this->classFormatter->getRequiredType());
	size_t currentRow = 0;
	for (std::vector<Sample>::iterator it = samples.begin(); it != samples.end(); ++it) {	
		//Calculating props
		float* row = samplesMat.ptr<float>(currentRow);
		std::vector<cv::Point> contour = FeatureProps::findMaxContour(it->image);
		row[0] = FeatureProps::calcSolidity(contour);
		row[1] = FeatureProps::calcContourPerimeter(contour);
		//Processing image class
		this->classFormatter->format(it->classId).row(0).copyTo(classesMat.row(currentRow));
		++currentRow;
	}
	return cv::ml::TrainData::create(samplesMat, cv::ml::ROW_SAMPLE, classesMat);
}

cv::Mat PropsSampleFormatter::format(cv::Mat& sample) {
	cv::Mat sampleMat(1, 2, CV_32FC1);
	std::vector<cv::Point> contour = FeatureProps::findMaxContour(sample);
	sampleMat.at<float>(0, 0) = FeatureProps::calcSolidity(contour);
	sampleMat.at<float>(0, 1) = FeatureProps::calcContourPerimeter(contour);
	return sampleMat;
}

PropsSampleFormatter::~PropsSampleFormatter()
{
	SampleFormatter::~SampleFormatter();
}
