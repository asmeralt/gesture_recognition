#pragma once
#include <exception>

class no_contours_exception :
	public std::exception
{
public:
	no_contours_exception();

	virtual const char* what() const throw();

	~no_contours_exception();
};

