//
// Created by Kavi Dey on 12/22/22.
//

#include <random>
#include <iostream>
#include "implot.h"
#include "util.h"

float lerp(float a, float b, float t) {
    return a * (1.0 - t) + (b * t);
}

float remap(float value, float istart, float istop, float ostart, float ostop) {
return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

std::random_device rd;
std::default_random_engine eng(rd());

std::uniform_real_distribution<> uniform_dist(0.0, 1.0);
std::normal_distribution<> normal_dist(0.0, 1.0);

float randUniform() {
    return uniform_dist(eng);
}

void randArray(float *randArr, int length, float (*randFunc)()) {
    float rand[length];

    for (int i = 0; i < length; i++) {
        randArr[i] = randFunc();
    }
}

float randNormal() {
    return normal_dist(eng);
}

float dist(float x1, float y1, float x2, float y2) {
    return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

void normalizeArray(float *array, int length, float n) {
    float sum = 0;
    sum = std::accumulate(array, array+length, sum);
    float divisor = (sum * n) / sum;

    for (int i = 0; i < length; i++) {
        array[i] /= divisor;
    }
}

void addToArray(float *array, int length, float num) {
    for (int i = 0; i < length; i++) {
        array[i] += num;
    }
}
