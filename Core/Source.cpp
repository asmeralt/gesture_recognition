#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\highgui.hpp"
#include "opencv2\imgproc.hpp"
#include "opencv2\videoio.hpp"

std::vector<cv::Scalar> calibration(cv::Mat& calibrationRegion, float alpha, cv::Mat& histogramDstImage);
std::vector<cv::Scalar> calibration(cv::Mat& calibrationRegion, float alpha);
std::vector<cv::Scalar> calcColorRange(cv::Mat hist1, cv::Mat hist2, float alpha);
std::vector<uchar> calcChannelRange(int mode, int dispertion, float alpha);
void calcChannelHistograms(cv::Mat& calibrationRegion, cv::Mat& cbHistOut, cv::Mat& crHistOut);
void calcHistogram(cv::Mat& src, cv::Mat& dst);
void histProperties(cv::Mat& histogram, int* mode, int* dispersion);
void plotHistogram(cv::Mat& histogram, cv::Scalar color, cv::Mat& dst);

// UI names consts
const std::string windowName = "Gesture recognition";

// Controls for a frame rate
const int frameCaptureDelayMillis = 40;  // approx 25 frames per second

// Calibration
const int calibrationSquareSize = 100;
const int calibrationSquareThickness = 3;

int main(int argc, char** argv) {
	cv::VideoCapture webcam;
	cv::Mat frame;
	cv::Mat mask;
	cv::Mat masked_frame;

	webcam.open(0);
	if (!webcam.isOpened()) { // check whereas webcam is available
		std::cout << "Can not open a video stream from your webcam";
		return -1;
	}

	cv::namedWindow(windowName);

	while (true) {
		masked_frame.release();
		webcam >> frame; // reading webcam
		if (frame.empty() || !webcam.isOpened()) { // check if webcam stream is not emtpy or closed
			std::cout << "Can not read a video stream from your webcam";
			return -1;
		}

		cv::cvtColor(frame, frame, cv::COLOR_BGR2YCrCb); // convert BGR frame to GrayScale

		cv::rectangle(frame, cv::Rect((frame.cols - calibrationSquareSize) / 2, (frame.rows - calibrationSquareSize) / 2, calibrationSquareSize + calibrationSquareThickness, calibrationSquareSize + calibrationSquareThickness), cv::Scalar(0, 0, 255), calibrationSquareThickness);
		cv::Mat calibRegion(frame, cv::Rect((frame.cols - calibrationSquareSize) / 2 + calibrationSquareThickness, (frame.rows - calibrationSquareSize) / 2 + calibrationSquareThickness, calibrationSquareSize - calibrationSquareThickness, calibrationSquareSize - calibrationSquareThickness));

		cv::Mat calibHistImage(400, 512, CV_8UC3, cv::Scalar(0, 0, 0));

		float alpha = 0.7;

		std::vector<cv::Scalar> range = calibration(calibRegion, alpha, calibHistImage);

		cv::inRange(frame, range[0], range[1], mask);

		cv::imshow(windowName, frame); // display resulted frame
		cv::imshow("Mask", mask); // display resulted frame
		cv::imshow("calcHist Demo", calibHistImage);

		// Break while loop when Esc is pressed
		if (cv::waitKey(frameCaptureDelayMillis) == 27) {
			break;
		}
	}

	return 0;
}

std::vector<cv::Scalar> calibration(cv::Mat& calibrationRegion, float alpha, cv::Mat& histogramDstImage) {
	cv::Mat crHist, cbHist;
	calcChannelHistograms(calibrationRegion, cbHist, crHist);
	plotHistogram(cbHist, cv::Scalar(255, 225, 20), histogramDstImage);
	plotHistogram(crHist, cv::Scalar(74, 255, 255), histogramDstImage);

	return calcColorRange(cbHist, crHist, alpha);
}

std::vector<cv::Scalar> calibration(cv::Mat& calibrationRegion, float alpha) {
	cv::Mat cbHist, crHist;
	calcChannelHistograms(calibrationRegion, cbHist, crHist);

	return calcColorRange(cbHist, crHist, alpha);
}

std::vector<cv::Scalar> calcColorRange(cv::Mat hist1, cv::Mat hist2, float alpha) {
	int hist1Mode, hist1Dispertion, hist2Mode, hist2Dispertion;
	histProperties(hist1, &hist1Mode, &hist1Dispertion);
	histProperties(hist2, &hist2Mode, &hist2Dispertion);

	std::vector<uchar> channel1Range = calcChannelRange(hist1Mode, hist1Dispertion, alpha);
	std::vector<uchar> channel2Range = calcChannelRange(hist2Mode, hist2Dispertion, alpha);

	cv::Scalar lowerb = cv::Scalar(0, channel1Range[0], channel2Range[0]);
	cv::Scalar upperb = cv::Scalar(UCHAR_MAX, channel1Range[1], channel2Range[1]);

	std::vector<cv::Scalar> skinColorRange(2);
	skinColorRange[0] = lowerb;
	skinColorRange[1] = upperb;
	return skinColorRange;
}

std::vector<uchar> calcChannelRange(int mode, int dispertion, float alpha) {
	std::vector<uchar> range(2);
	range[0] = mode - alpha * std::sqrt(dispertion);
	range[1] = mode + alpha * std::sqrt(dispertion);
	return range;
}

void calcChannelHistograms(cv::Mat& calibrationRegion, cv::Mat& cbHistOut, cv::Mat& crHistOut) {
	std::vector<cv::Mat> bgr_planes;
	cv::split(calibrationRegion, bgr_planes);
	calcHistogram(bgr_planes[1], cbHistOut);
	calcHistogram(bgr_planes[2], crHistOut);
}

void calcHistogram(cv::Mat& src, cv::Mat& dst) {
	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };
	bool uniform = true;
	bool accumulate = false;

	cv::calcHist(&src, 1, 0, cv::Mat(), dst, 1, &histSize, &histRange, uniform, accumulate);
}

void histProperties(cv::Mat& histogram, int* mode, int* dispersion) {
	double min, max;
	cv::Point minIdx, maxIdx;

	cv::minMaxLoc(histogram, &min, &max, &minIdx, &maxIdx); // Ask about effiency of adding a mask!!!!

	double disp = 0;
	float* histVal = histogram.col(0).ptr<float>();
	int totalVal = histogram.rows * histogram.cols;	

	for (int i = 0; i < histogram.rows; ++i) {
		disp += std::pow(i - maxIdx.y, 2) * histVal[i] / totalVal;
	}

	*mode = maxIdx.y;
	*dispersion = std::ceil(disp);

}

void plotHistogram(cv::Mat& histogram, cv::Scalar color, cv::Mat& dst) {
	int histSize = histogram.rows;
	int bin_w = cvRound((double)dst.cols / histSize);
	cv::Mat normHist(dst.rows, 1, dst.type());
	cv::normalize(histogram, normHist, 0, dst.rows, cv::NORM_MINMAX, -1, cv::Mat());
	for (int i = 1; i < histSize; i++)
	{
		line(dst, cv::Point(bin_w*(i - 1), dst.rows - cvRound(normHist.at<float>(i - 1))),
			cv::Point(bin_w*(i), dst.rows - cvRound(normHist.at<float>(i))),
			color, 2, 8, 0);
	}
}