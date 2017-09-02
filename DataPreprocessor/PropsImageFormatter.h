#pragma once
#include "ImageFormatter.h"
class PropsImageFormatter :
	public ImageFormatter
{
private:
	std::vector<cv::Point>(*contourFinder)(cv::Mat&);

	std::vector<float(*)(std::vector<cv::Point>&)> props;
public:
	PropsImageFormatter(std::vector<cv::Point>(contourFinder)(cv::Mat&), std::vector<float(*)(std::vector<cv::Point>&)>& props);

	virtual cv::Mat format(cv::Mat& image);

	virtual int getRequiredColumns();

	virtual int getRequiredType();

	~PropsImageFormatter();
};

