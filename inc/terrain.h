#pragma once

#include "object.h"
#include <array>

class Terrain : public Object {
  public:
	std::array<std::array<float,10>, 10> HeightMap;
	Terrain();
};