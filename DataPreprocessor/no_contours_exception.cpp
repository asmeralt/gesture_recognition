#include "no_contours_exception.h"

no_contours_exception::no_contours_exception()
{
}

const char* no_contours_exception::what() const throw()
{
	return "No contours found";
}

no_contours_exception::~no_contours_exception()
{
}
