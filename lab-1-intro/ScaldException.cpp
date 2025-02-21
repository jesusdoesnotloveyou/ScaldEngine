#include "ScaldException.h"

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
