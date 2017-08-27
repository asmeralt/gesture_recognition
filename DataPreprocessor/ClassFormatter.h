#pragma once
#include <opencv2\core.hpp>

class ClassFormatter
{
public:
	
	virtual cv::Mat format(int classId) = 0;

	virtual int formatBack(cv::Mat&) = 0;

	virtual int getRequiredColumns() = 0;

	virtual int getRequiredType() = 0;

};

