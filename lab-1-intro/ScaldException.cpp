#include "ScaldException.h"
//#include <comdef.h>

ScaldException::ScaldException(int line, const char* file)
    :
    _line(line),
    _file(file)
{}

const char* ScaldException::what() const
{
    return nullptr;
}

WindowException::WindowException(int line, const char* file, HRESULT hr)
    :
    ScaldException(line, file),
    _hr(hr)
{}

const char* WindowException::what() const
{
    return nullptr;
}
//
//void ErrorLogger::Log(std::string message)
//{
//    std::string error_message = "Error: " + message;
//    MessageBox(nullptr, error_message.c_str(), "Error", MB_ICONERROR);
//}
//
//void ErrorLogger::Log(HRESULT hr, std::string message)
//{
//    _com_error error(hr);
//    std::string error_message = "Error: " + message + std::string(error.ErrorMessage());
//    MessageBox(nullptr, error_message.c_str(), "Error", MB_ICONERROR);
//}