#pragma once
#include "ClassFormatter.h"

class NumClassFormatter:
	public ClassFormatter
{
private :
	int requiredType;
public:
	NumClassFormatter();

	NumClassFormatter(int requiredType);

	virtual cv::Mat format(int classId);

	virtual int formatBack(cv::Mat& classVec);

	virtual int getRequiredColumns();

	virtual int getRequiredType();

	~NumClassFormatter();
};

