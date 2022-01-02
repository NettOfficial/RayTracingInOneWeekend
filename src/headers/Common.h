#pragma once

#ifndef COMMON_H
#define COMMON_F

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <cstdlib>


//Usings
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

//Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

//Utility Functions
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

//Common Headers
#include "Ray.h"
#include "Vector3.h"

inline float random_float()
{
    return rand() / (RAND_MAX + 1.f);
}

/*
inline float random_float() 
{
    static std::uniform_real_distribution<float> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
*/

inline float random_float(float min, float max)
{
    return min + (max - min) * random_float();
}

inline float clamp(float x, float min, float max)
{
    if (x < min) 
        return min;
    if (x > max) 
        return max;
    return x;
}

#endif // !COMMON_H
