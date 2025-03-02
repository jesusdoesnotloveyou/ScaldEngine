#pragma once

#include "PrimitiveGeometry.h"

class Star : public PrimitiveGeometry
{
public:
	Star();
	Star(std::vector<Vertex>& v, std::vector<int>& i);

	virtual ~Star() override;

};