#pragma once
#include <opencv2\core.hpp>

class FeatureProps
{
public:

	static std::vector<cv::Point> findMaxContour(cv::Mat& sample);

	static std::vector<std::vector<cv::Point>> findMaxContours(std::vector<cv::Mat>& samples);

	static float calcMinBoundingRectArea(std::vector<cv::Point>& contour);

	static float calcMinBoundingRectPerimeter(std::vector<cv::Point>& contour);

	static float calcExtent(std::vector<cv::Point>& contour);

	static float calcSolidity(std::vector<cv::Point>& contour);

	static float calcContourArea(std::vector<cv::Point>& contour);

	static float calcContourPerimeter(std::vector<cv::Point>& contour);

	static std::vector<float> calcMinBoundingRectArea(std::vector<std::vector<cv::Point>>& contours);

	static std::vector<float> calcMinBoundingRectPerimeter(std::vector<std::vector<cv::Point>>& contours);

	static std::vector<float> calcExtent(std::vector<std::vector<cv::Point>>& contours);

	static std::vector<float> calcSolidity(std::vector<std::vector<cv::Point>>& contours);

	static std::vector<float> calcContourArea(std::vector<std::vector<cv::Point>>& contours);

	static std::vector<float> calcContourPerimeter(std::vector<std::vector<cv::Point>>& contours);

protected:
	static std::vector<float> calcProps(std::vector<std::vector<cv::Point>>& contours, float(calc)(std::vector<cv::Point>&));	
	};

