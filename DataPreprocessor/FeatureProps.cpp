#include "FeatureProps.h"
#include <opencv2\imgproc.hpp>
#include "no_contours_exception.h"

std::vector<cv::Point> FeatureProps::findMaxContour(cv::Mat& sample) {
	std::vector<std::vector<cv::Point>> sampleContours;
	cv::findContours(sample, sampleContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
	if (sampleContours.size() > 0) {
		return std::max_element(sampleContours.begin(), sampleContours.end(), [](std::vector<cv::Point>& a, std::vector<cv::Point>& b) {
			return cv::contourArea(a) < cv::contourArea(b);
		})[0];
	}
	else {
		throw no_contours_exception();
	}
}

std::vector<std::vector<cv::Point>> FeatureProps::findMaxContours(std::vector<cv::Mat>& samples) {
	std::vector<std::vector<cv::Point>> samplesMaxCountours;
	samplesMaxCountours.reserve(samples.size());
	for (int i = 0; i < samples.size(); ++i) {
		samplesMaxCountours.push_back(findMaxContour(samples[i]));
	}
	return samplesMaxCountours;
}

float FeatureProps::calcMinBoundingRectArea(std::vector<cv::Point>& contour) {
	cv::Size rectSize = cv::minAreaRect(contour).size;
	return rectSize.height * rectSize.width;
}

float FeatureProps::calcMinBoundingRectPerimeter(std::vector<cv::Point>& contour) {
	cv::Size rectSize = cv::minAreaRect(contour).size;
	return 2 * (rectSize.height + rectSize.width);
}

float FeatureProps::calcExtent(std::vector<cv::Point>& contour) {
	return (float)(cv::contourArea(contour) / calcMinBoundingRectArea(contour));
}

float FeatureProps::calcSolidity(std::vector<cv::Point>& contour) {
	std::vector<cv::Point> convexHull;
	cv::convexHull(contour, convexHull);
	return (float)(cv::contourArea(contour) / cv::contourArea(convexHull));
}

float FeatureProps::calcContourArea(std::vector<cv::Point>& contour) {
	return (float)(cv::contourArea(contour) / calcMinBoundingRectArea(contour));
}

float FeatureProps::calcContourPerimeter(std::vector<cv::Point>& contour) {
	return (float)(cv::arcLength(contour, true) / calcMinBoundingRectPerimeter(contour));
}

std::vector<float> FeatureProps::calcMinBoundingRectArea(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcMinBoundingRectArea);
}

std::vector<float> FeatureProps::calcMinBoundingRectPerimeter(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcMinBoundingRectPerimeter);
}

std::vector<float> FeatureProps::calcExtent(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcExtent);
}

std::vector<float> FeatureProps::calcSolidity(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcSolidity);
}

std::vector<float> FeatureProps::calcContourArea(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcContourArea);
}

std::vector<float> FeatureProps::calcContourPerimeter(std::vector<std::vector<cv::Point>>& contours) {
	return calcProps(contours, calcContourPerimeter);
}

std::vector<float> FeatureProps::calcProps(std::vector<std::vector<cv::Point>>& contours, float(calc)(std::vector<cv::Point>&)) {
	std::vector<float> props(contours.size());
	for (int i = 0; i < contours.size(); ++i) {
		props[i] = calc(contours[i]);
	}
	return props;
}