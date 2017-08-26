#pragma once
#include <opencv2\core.hpp>
struct Sample
{
	cv::Mat image;
	int classId;
	Sample(cv::Mat image, int classId) : image(image), classId(classId)
	{
	}
};

