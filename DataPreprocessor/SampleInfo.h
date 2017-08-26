#pragma once
#include <string>
struct SampleInfo 
{
	std::string file;
	unsigned char classId;
	SampleInfo(std::string file, unsigned char classId) : file(file), classId(classId)
	{
	}
};

