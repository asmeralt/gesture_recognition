#pragma once
#include <list>
#include <string>
#include <opencv2\core.hpp>
#include "SampleInfo.h"
#include "Sample.h"

class SampleLoader
{

public:

	void load(std::string& batchFileName, std::vector<Sample>& samplesOut, int& maxClassId);

private:

	static void readBatchFile(std::string& batchFileName, std::vector<SampleInfo>& sampleInfosOut, int& maxClassIdOut);
};

