#include <iostream>
#include <fstream>
#include <list>
#include <opencv2\core.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\ml.hpp>

#define SAMPLE_SIZE 16
#define THRESHOLD 175
#define MAX_TRAIN_ITER 100
#define PRECISION 0.0001

struct DataSampleInfo {
	int classId;
	std::string file;

	DataSampleInfo(int classId, std::string file) :classId(classId), file(file) {

	}
};

int readSamples(std::vector<cv::Mat>& samples, std::vector<uchar>& classes, std::string batchFileName) {
	std::ifstream batchFile;
	batchFile.open(batchFileName);

	int classId;
	int maxClassId = INT32_MIN;
	std::string file;
	std::list<DataSampleInfo> sampleInfos;
	while (batchFile >> classId >> file) {
		sampleInfos.push_back(DataSampleInfo(classId, file));
		if (maxClassId < classId) {
			maxClassId = classId;
		}
	}
	batchFile.close();
	samples.reserve(sampleInfos.size());
	classes.reserve(sampleInfos.size());
	for (std::list<DataSampleInfo>::iterator it = sampleInfos.begin(); it != sampleInfos.end(); ++it) {
		//Processing image sample
		samples.push_back(cv::imread(it->file, cv::IMREAD_GRAYSCALE));
		//Processing image class
		classes.push_back(it->classId);
	}
	return maxClassId;
}

int loadSamples(std::vector<cv::Mat>& samples, std::vector<uchar>& classes, std::string batchFileName) {
	return readSamples(samples, classes, batchFileName);
}

void findMaxContours(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>>& contours) {
	contours.reserve(samples.size());
	for (int i = 0; i < samples.size(); ++i) {
		std::vector<std::vector<cv::Point>> sampleContours;
		cv::findContours(samples[i], sampleContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
		contours.push_back(std::max_element(sampleContours.begin(), sampleContours.end(), [](std::vector<cv::Point>& a, std::vector<cv::Point>& b) {
			return cv::contourArea(a) < cv::contourArea(b);
		})[0]);
	}
}

void calcProps(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>> contours, std::vector<uchar>& classes, std::vector<float>& props, float(calc)(std::vector<cv::Point>&)) {
	props.resize(samples.size());
	for (int i = 0; i < samples.size(); ++i) {
		props[i] = calc(contours[i]);
	}
}

float calcMinBoundingRectArea(std::vector<cv::Point> contour) {
	cv::Size rectSize = cv::minAreaRect(contour).size;
	return rectSize.height * rectSize.width;
}

float calcMinBoundingRectPerimeter(std::vector<cv::Point> contour) {
	cv::Size rectSize = cv::minAreaRect(contour).size;
	return 2 * (rectSize.height + rectSize.width);
}

void calcExtent(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>> contours, std::vector<uchar>& classes, std::vector<float>& extent) {
	calcProps(samples, contours, classes, extent, [](std::vector<cv::Point>& contour) {
		return (float)(cv::contourArea(contour) / calcMinBoundingRectArea(contour));
	});
}

void calcSolidity(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>> contours, std::vector<uchar>& classes, std::vector<float>& solidity) {
	calcProps(samples, contours, classes, solidity, [](std::vector<cv::Point>& contour) {
		std::vector<cv::Point> hull;
		cv::convexHull(contour, hull);
		return (float)(cv::contourArea(contour) / cv::contourArea(hull));
	});
}

void calcContourArea(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>> contours, std::vector<uchar>& classes, std::vector<float>& area) {
	calcProps(samples, contours, classes, area, [](std::vector<cv::Point>& contour) {
		return (float)(cv::contourArea(contour) / calcMinBoundingRectArea(contour));
	});
}

void calcContourPerimeter(std::vector<cv::Mat>& samples, std::vector<std::vector<cv::Point>> contours, std::vector<uchar>& classes, std::vector<float>& perimeter) {
	calcProps(samples, contours, classes, perimeter, [](std::vector<cv::Point>& contour) {
		return (float)(cv::arcLength(contour, true)/calcMinBoundingRectPerimeter(contour));
	});
}

void plotProp1D(std::vector<uchar>& classes, std::vector<float>& props, std::string windowName) {
	int widht = 400;
	int height = 300;
	cv::Mat plot(height + 1, widht + 1, CV_8UC3, cv::Scalar(0, 0, 0));
	float propMax = std::max_element(props.begin(), props.end())[0];
	for (int i = 0; i < classes.size(); ++i) {
		int x = props[i] * widht / propMax;
		if (classes[i] == 1) {
			int y = 0.25 * height;
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
		}
		else if (classes[i] == 2) {
			int y = 0.5 * height;
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
		}
		else {
			int y = 0.75 * height;
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
		}
	}
	cv::imshow(windowName, plot);
}

void plotProps2D(std::vector<uchar>& classes, std::vector<float>& propsX, std::vector<float>& propsY, std::string windowName) {
	int widht = 400;
	int height = 300;
	cv::Mat plot(height + 1, widht + 1, CV_8UC3, cv::Scalar(0, 0, 0));
	float propXMax = std::max_element(propsX.begin(), propsX.end())[0];
	float propYMax = std::max_element(propsY.begin(), propsY.end())[0];
	for (int i = 0; i < classes.size(); ++i) {
		int x = propsX[i] * widht / propXMax;
		int y = propsY[i] * height / propYMax;
		if (classes[i] == 1) {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
		}
		else if (classes[i] == 2) {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
		}
		else {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
		}
	}
	cv::imshow(windowName, plot);
}

int main(int argc, char** argv) {
	std::vector<cv::Mat> samples;
	std::vector<uchar> classes;

	std::clog << "Loading samples: ";
	loadSamples(samples, classes, argv[1]);
	std::clog << " DONE." << std::endl;

	std::clog << "Finding contours: ";
	std::vector<std::vector<cv::Point>> contours;
	findMaxContours(samples, contours);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating extent: ";
	std::vector<float> extent;
	calcExtent(samples, contours, classes, extent);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating solidity: ";
	std::vector<float> solidity;
	calcSolidity(samples, contours, classes, solidity);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating contour area: ";
	std::vector<float> area;
	calcContourArea(samples, contours, classes, area);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating contour perimetr: ";
	std::vector<float> perimeter;
	calcContourPerimeter(samples, contours, classes, perimeter);
	std::clog << " DONE." << std::endl;

	std::clog << "Plotting properties: ";
	//plotProp1D(classes, extent, "Extents");
	//plotProp1D(classes, solidity, "Solidity");
	//plotProp1D(classes, area, "Area");
	//plotProp1D(classes, perimeter, "Perimeter");
	plotProps2D(classes, extent, solidity, "Extents.Solidity");
	plotProps2D(classes, extent, area, "Extents.Area");
	plotProps2D(classes, extent, perimeter, "Extents.Perimeter");
	plotProps2D(classes, solidity, area, "Solidity.Area");
	plotProps2D(classes, solidity, perimeter, "Solidity.Perimeter");
	plotProps2D(classes, area, perimeter, "Area.Perimeter");
	std::clog << " DONE." << std::endl;

	cv::waitKey(0);
	return 0;
}

#undef PRECISION
#undef SAMPLE_SIZE
#undef MAX_TRAIN_ITER
#undef THRESHOLD 