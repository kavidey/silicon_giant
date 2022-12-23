//
// Created by Kavi Dey on 12/22/22.
//

#include <random>
#include <iostream>
#include "util.h"

float lerp(float a, float b, float t) {
    return a * (1.0 - t) + (b * t);
}

std::random_device rd;
std::default_random_engine eng(rd());

std::uniform_real_distribution<> uniform_dist(0.0, 1.0);
std::normal_distribution<> normal_dist(0.0, 1.0);

float randUniform() {
    return uniform_dist(eng);
}

float randNormal() {
    return normal_dist(eng);
}
