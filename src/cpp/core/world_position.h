#pragma once

#include "data_types.h"

namespace prank {

// 2.5D 坐标工具
inline godot::Vector2 world_to_screen(int32_t x, int32_t y, int32_t z, float z_scale = 0.5f) {
    return godot::Vector2(
        static_cast<float>(x),
        static_cast<float>(z) * z_scale - static_cast<float>(y)
    );
}

inline int32_t get_sort_order(int32_t z) {
    return -z;
}

} // namespace prank
