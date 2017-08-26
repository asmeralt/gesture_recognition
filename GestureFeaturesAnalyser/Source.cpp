#include <opencv2\highgui.hpp>
#include <iostream>
#include "SampleLoader.h"
#include "FeatureProps.h"

void plotProp1D(std::vector<Sample>& samples, std::vector<float>& props, std::string windowName) {
	int widht = 400;
	int height = 300;
	cv::Mat plot(height + 1, widht + 1, CV_8UC3, cv::Scalar(0, 0, 0));
	float propMax = std::max_element(props.begin(), props.end())[0];
	for (int i = 0; i < samples.size(); ++i) {
		int x = props[i] * widht / propMax;
		if (samples[i].classId == 1) {
			int y = 0.25 * height;
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
		}
		else if (samples[i].classId == 2) {
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

void plotProps2D(std::vector<Sample>& samples, std::vector<float>& propsX, std::vector<float>& propsY, std::string windowName) {
	int widht = 400;
	int height = 300;
	cv::Mat plot(height + 1, widht + 1, CV_8UC3, cv::Scalar(0, 0, 0));
	float propXMax = std::max_element(propsX.begin(), propsX.end())[0];
	float propYMax = std::max_element(propsY.begin(), propsY.end())[0];
	for (int i = 0; i < samples.size(); ++i) {
		int x = propsX[i] * widht / propXMax;
		int y = propsY[i] * height / propYMax;
		if (samples[i].classId == 1) {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 255, 0);
		}
		else if (samples[i].classId == 2) {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 255);
		}
		else {
			plot.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0);
		}
	}
	cv::imshow(windowName, plot);
}

int main(int argc, char** argv) {
	std::clog << "Loading samples: "; 
	SampleLoader loader;
	std::vector<Sample> samples;
	int maxClassId;
	loader.load(std::string(argv[1]), samples, maxClassId);
	std::clog << " DONE." << std::endl;

	std::vector<cv::Mat> sapmplesImages(samples.size());
	std::transform(samples.begin(), samples.end(), sapmplesImages.begin(), [](Sample& sample){
		return sample.image;
	});

	std::clog << "Finding contours: ";
	std::vector<std::vector<cv::Point>> contours = FeatureProps::findMaxContours(sapmplesImages);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating extent: ";
	std::vector<float> extent = FeatureProps::calcExtent(contours);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating solidity: ";
	std::vector<float> solidity = FeatureProps::calcSolidity(contours);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating contour area: ";
	std::vector<float> area = FeatureProps::calcContourArea(contours);
	std::clog << " DONE." << std::endl;

	std::clog << "Calculating contour perimeter: ";
	std::vector<float> perimeter =FeatureProps::calcContourPerimeter(contours);
	std::clog << " DONE." << std::endl;

	std::clog << "Plotting properties: ";
	//plotProp1D(classes, extent, "Extents");
	//plotProp1D(classes, solidity, "Solidity");
	//plotProp1D(classes, area, "Area");
	//plotProp1D(classes, perimeter, "Perimeter");
	plotProps2D(samples, extent, solidity, "Extents.Solidity");
	plotProps2D(samples, extent, area, "Extents.Area");
	plotProps2D(samples, extent, perimeter, "Extents.Perimeter");
	plotProps2D(samples, solidity, area, "Solidity.Area");
	plotProps2D(samples, solidity, perimeter, "Solidity.Perimeter");
	plotProps2D(samples, area, perimeter, "Area.Perimeter");
	std::clog << " DONE." << std::endl;

	cv::waitKey(0);
	return 0;
}