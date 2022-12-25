//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_UTIL_H
#define SILICON_GIANT_UTIL_H

#include <iostream>

/// Linear Interpolation
/// \param a starting value
/// \param b ending value
/// \param t percentage (0-1)
/// \return
float lerp(float a, float b, float t);

/// Remaps the range of a value
/// \param value value to remap
/// \param istart input range lower bound
/// \param istop input range higher bound
/// \param ostart output range lower bound
/// \param ostop output range higher bound
/// \return
float remap(float value, float istart, float istop, float ostart, float ostop);

float dist(float x1, float y1, float x2, float y2);

///
/// \return random float between 0 and 1 (uniform distribution)
float randUniform();

///
/// \return random float with a normal distribution centered at 0 and mean of 1
float randNormal();

#if DEBUG_LEVEL == 1
#define DEBUG(x) std::cout << x << std::endl;
#else
#define DEBUG(x)
#endif

#endif //SILICON_GIANT_UTIL_H
