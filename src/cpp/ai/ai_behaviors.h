#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

// AI 行为模板数据
struct AIBehaviorTemplate {
    godot::String name;
    int aggressive_weight = 50;
    int defensive_weight = 30;
    int movement_weight = 20;
    int combo_tendency = 40;
    int special_tendency = 30;
    int preferred_distance = 60;
    int reaction_speed = 10; // frames
};

} // namespace prank
