//
// Created by Kavi Dey on 12/24/22.
//

#ifndef SILICON_GIANT_UI_H
#define SILICON_GIANT_UI_H

#include "imgui.h"
#include "implot.h"

/// Copied from: void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size);
void Sparkline(const char* id, const float* values, int count, float min_v, float max_v, int offset, const ImVec4& col, const ImVec2& size);

#endif //SILICON_GIANT_UI_H
