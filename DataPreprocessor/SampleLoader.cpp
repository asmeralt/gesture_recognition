#include "SampleLoader.h"
#include <list>
#include <fstream>
#include <iterator>
#include <opencv2\imgproc.hpp>
#include <opencv2\highgui.hpp>

void SampleLoader::load(std::string& batchFileName, std::vector<Sample>& samplesOut, int& maxClassId) {
	std::vector<SampleInfo> sampleInfos;
	readBatchFile(batchFileName, sampleInfos, maxClassId);
	for (std::vector<SampleInfo>::iterator it = sampleInfos.begin(); it != sampleInfos.end(); ++it) {
		samplesOut.push_back(Sample(cv::imread(it->file, cv::IMREAD_GRAYSCALE), it->classId));
	}
}

void SampleLoader::readBatchFile(std::string& batchFileName, std::vector<SampleInfo>& sampleInfosOut, int& maxClassIdOut) {
	std::ifstream batchFile;
	int classId;
	std::string file;
	std::list<SampleInfo> sampleInfos;
	maxClassIdOut = 0;
	batchFile.open(batchFileName);
	while (batchFile >> classId >> file) {
		sampleInfos.push_back(SampleInfo(file, classId));
		if (maxClassIdOut < classId) {
			maxClassIdOut = classId;
		}
	}
	batchFile.close();
	sampleInfosOut.reserve(sampleInfos.size());
	std::copy(sampleInfos.begin(), sampleInfos.end(), std::back_inserter(sampleInfosOut));
}
