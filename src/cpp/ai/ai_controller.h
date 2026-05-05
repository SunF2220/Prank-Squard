#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

namespace prank {

// 角色 AI 控制器
class AIController {
public:
    enum Difficulty {
        EASY = 0,
        NORMAL = 1,
        HARD = 2
    };

    enum Behavior {
        AGGRESSIVE = 0,
        DEFENSIVE = 1,
        ZONER = 2,
        HIT_AND_RUN = 3
    };

    AIController();
    ~AIController() = default;

    void set_difficulty(Difficulty diff) { difficulty_ = diff; }
    void set_behavior(Behavior beh) { behavior_ = beh; }

    // Returns a bitmask of inputs for the AI fighter
    uint8_t get_inputs(int32_t ai_x, int32_t ai_y, int32_t ai_z,
                       int32_t enemy_x, int32_t enemy_y, int32_t enemy_z,
                       float delta);
    uint8_t get_buttons() const { return buttons_; }

private:
    Difficulty difficulty_ = NORMAL;
    Behavior behavior_ = AGGRESSIVE;
    uint8_t directions_ = 0;
    uint8_t buttons_ = 0;

    float decision_timer_ = 0.0f;
    float decision_interval_ = 0.3f;

    void decide_easy(int32_t dx, int32_t dy, int32_t dz);
    void decide_normal(int32_t dx, int32_t dy, int32_t dz);
    void decide_hard(int32_t dx, int32_t dy, int32_t dz);
};

} // namespace prank
