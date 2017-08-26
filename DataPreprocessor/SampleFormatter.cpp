#include "SampleFormatter.h"

SampleFormatter::SampleFormatter(ClassFormatter* classFormatter): classFormatter(classFormatter)
{
}

SampleFormatter::~SampleFormatter()
{
	delete classFormatter;
}