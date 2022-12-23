//
// Created by Kavi Dey on 12/22/22.
//

#ifndef SILICON_GIANT_UTIL_H
#define SILICON_GIANT_UTIL_H

/// Linear Interpolation
/// \param a starting value
/// \param b ending value
/// \param t percentage (0-1)
/// \return
float lerp(float a, float b, float t);

///
/// \return random float between 0 and 1 (uniform distribution)
float rand_float();

#endif //SILICON_GIANT_UTIL_H
