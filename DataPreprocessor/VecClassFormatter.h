#pragma once
#include "ClassFormatter.h"
class VecClassFormatter :
	public ClassFormatter
{
private:
	int requiredType;
	int maxClassId;
public:
	VecClassFormatter(int maxClassId);

	VecClassFormatter(int requiredType, int maxClassId);

	virtual cv::Mat format(int classId);

	virtual int formatBack(cv::Mat& classVec);

	virtual int getRequiredColumns();

	virtual int getRequiredType();

	~VecClassFormatter();
};

