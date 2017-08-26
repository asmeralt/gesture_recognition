#pragma once
#include "ImageFormatter.h"

class RawImageFormatter:
	public ImageFormatter
{
private:
	const cv::Size size;
public:
	RawImageFormatter(cv::Size size);

	virtual cv::Mat format(cv::Mat& image);

	virtual int getRequiredColumns();

	virtual int getRequiredType();

	~RawImageFormatter();
};

