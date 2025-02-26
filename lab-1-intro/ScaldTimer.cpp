#include "ScaldTimer.h"

ScaldTimer::ScaldTimer()
{
    last = _CHRONO steady_clock::now();
}

float ScaldTimer::Mark()
{
    const auto old = last;
    last = _CHRONO steady_clock::now();
    const _CHRONO duration<float> frameRate = last - old;
    return frameRate.count();
}

float ScaldTimer::Peek() const
{
    return _CHRONO duration<float>(_CHRONO steady_clock::now() - last).count();
}
