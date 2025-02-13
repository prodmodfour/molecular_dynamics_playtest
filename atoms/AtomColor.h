#pragma once
#include <string>
#include <tuple>

namespace atoms {

typedef struct AtomColor
{
    std::string color_name;
    std::tuple<int, int, int, int> color_rgb_alpha;
} AtomColor;

}