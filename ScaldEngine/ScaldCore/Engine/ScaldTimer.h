#pragma once

class ScaldTimer
{
public:
	ScaldTimer();
	
	float TotalTime() const;  // in seconds
	float DeltaTime() const; // in seconds

	// Call before message loop.
	void Reset();
	// Call when unpaused.
	void Start();
	// Call when paused.
	void Stop();
	// Call every frame.
	void Tick();

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mStopped;
};