#pragma once
#include "ImageFormatter.h"
class PropsImageFormatter :
	public ImageFormatter
{
public:
	PropsImageFormatter();

	virtual cv::Mat format(cv::Mat& image);

	virtual int getRequiredColumns();

	virtual int getRequiredType();

	~PropsImageFormatter();
};

