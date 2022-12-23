//
// Created by Kavi Dey on 12/22/22.
//

#include "util.h"

float lerp(float a, float b, float t) {
    return a * (1.0 - t) + (b * t);
}