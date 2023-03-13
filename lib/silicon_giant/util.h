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

/// Fills an array with values from \p randFunc
/// \param length length of the array to generate
/// \param randFunc function that takes no inputs and returns a float
void randArray(float *randArr, int length, float (*randFunc)());

///
/// \return random float with a normal distribution centered at 0 and mean of 1
float randNormal();

/// Normalizes the array so that all of the entries sum to \p n
/// \param array the array to normalize
/// \param length the length of \p array
/// \param n what the sum of the array should be (default: 1.0)
void normalizeArray(float *array, int length, float n=1);

void addToArray(float *array, int length, float num);

#if DEBUG_LEVEL == 1
#define DEBUG(x) std::cout << x << std::endl;
#else
#define DEBUG(x)
#endif

#endif //SILICON_GIANT_UTIL_H
