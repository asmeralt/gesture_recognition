#include "CalibrationStateExecutor.h"

#define ALPHA_TRACKBAR_MAX_VALUE 50
#define ALPHA_TRACKBAR_COEFICIENT 3.0f / ALPHA_TRACKBAR_MAX_VALUE
#define ALPHA_TRACKBAR_INIT_VALUE 5

// public
CalibrationStateExecutor::CalibrationStateExecutor(SkinCalibrator& skinCalibrator, float alpha) :
	skinCalibrator(skinCalibrator),
	alpha(alpha),
	skinColorFilterRange(std::vector<cv::Scalar>({ cv::Scalar(0,0,0), cv::Scalar(UCHAR_MAX, UCHAR_MAX, UCHAR_MAX) })),
	alphaTrackbarValue(ALPHA_TRACKBAR_INIT_VALUE),
	alphaTrackbarMax(ALPHA_TRACKBAR_MAX_VALUE)
{
}

CalibrationStateExecutor::~CalibrationStateExecutor()
{
}

void CalibrationStateExecutor::execute(cv::Mat& frame) {
	cv::Rect calibrationRect = buildAndPlotAlertRectangle(frame);
	cv::Mat calibrationRegion;
	cv::cvtColor(frame(calibrationRect).clone(), calibrationRegion, cv::COLOR_BGR2YCrCb);
	cv::medianBlur(calibrationRegion, calibrationRegion, 5);
	bool channels[] = { false, true, true };
	float alphaArr[] = { alpha, alpha, alpha };

	std::vector<cv::Mat> histograms(3);
	this->skinColorFilterRange = skinCalibrator.calibrate(calibrationRegion, channels, alphaArr, histograms);
	showCalibrationHistograms(histograms);
}

std::vector<cv::Scalar> CalibrationStateExecutor::getSkinColorFilterRange() {
	return this->skinColorFilterRange;
}

// private
// TODO refactor this method: two actions in one method
cv::Rect CalibrationStateExecutor::buildAndPlotAlertRectangle(cv::Mat&  frame) {
	int squareSize = frame.rows / 4.5;
	int squareThickness = 2;
	cv::Rect alertRectangle((frame.cols - squareSize) / 2, (frame.rows - squareSize) / 2, squareSize + 2 * squareThickness, squareSize + 2 * squareThickness);
	cv::rectangle(frame, alertRectangle, cv::Scalar(0, 0, 255), squareThickness, cv::LINE_4);
	return cv::Rect((frame.cols - squareSize) / 2 + squareThickness, (frame.rows - squareSize) / 2 + squareThickness, squareSize, squareSize);
}

void CalibrationStateExecutor::showCalibrationHistograms(std::vector<cv::Mat>& histograms) {
	cv::Mat histogramsFrame(400, 512, CV_8UC3, cv::Scalar(0, 0, 0));
	plotCalibrationHistograms(histograms, histogramsFrame);

	//Create Trackbar on alpha (variance coeficient)
	cv::createTrackbar("Alpha", "Calibration histograms", &alphaTrackbarValue, alphaTrackbarMax, [](int value, void* userdata) {
		float* alpha = (float*)userdata;
		*alpha = 0.06f * value;
	}, &(this->alpha));
	cv::imshow("Calibration histograms", histogramsFrame);
}

void CalibrationStateExecutor::plotCalibrationHistograms(std::vector<cv::Mat> histograms, cv::Mat& dst) {
	std::vector<cv::Scalar> colors(3);
	colors[0] = cv::Scalar(255, 255, 255);
	colors[1] = cv::Scalar(255, 127, 127);
	colors[2] = cv::Scalar(127, 127, 255);
	float alphas[] = { alpha,alpha,alpha };
	skinCalibrator.plotHistograms(histograms, colors, alphas, dst);
}

#undef ALPHA_TRACKBAR_INIT_VALUE
#undef ALPHA_TRACKBAR_COEFICIENT
#undef ALPHA_TRACKBAR_MAX_VALUE