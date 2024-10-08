#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <limits>
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degress_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

float length_squared(const glm::vec3& v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

#include "ray.h"

#endif