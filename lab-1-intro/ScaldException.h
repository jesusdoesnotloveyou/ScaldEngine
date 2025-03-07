#pragma once

#include "ScaldCore/Engine/ScaldWindows.h"
#include <exception>
#include <string>

// temporary to threat errors
inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		//const char* const msg = (TEXT("Line: %d"), __LINE__, TEXT("File: %d"), __FILE__);
		throw std::exception{ /*msg*/};
	}
}

class ScaldException : public std::exception
{
public:
	ScaldException(int line, const char* file);
	virtual const char* what() const override;

private:
	int _line;
	std::string _file;
};

class WindowException : public ScaldException
{
public:
	WindowException(int line, const char* file, HRESULT hr);
	virtual const char* what() const override;

private:
	HRESULT _hr;
};

//class ErrorLogger
//{
//public:
//	static void Log(std::string message);
//	static void Log(HRESULT hr, std::string message);
//};