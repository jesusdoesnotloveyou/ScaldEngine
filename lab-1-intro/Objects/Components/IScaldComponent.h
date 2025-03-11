#pragma once

class IScaldComponent
{
public:
	// if IScaldComponent has a number of members that should be initialized from derived
	IScaldComponent() {};

	virtual ~IScaldComponent() = 0 {};
	virtual void Update(float DeltaTime) = 0;
};