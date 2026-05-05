#pragma once

#include "../core/data_types.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace prank {

// 平衡性校验器 - 确保生成的角色在合理范围内
class BalanceValidator {
public:
    BalanceValidator();
    ~BalanceValidator() = default;

    struct BalanceReport {
        bool is_valid = true;
        godot::String warnings;
        int total_damage_output = 0;  // 理想连段总伤害
        float power_rating = 0.0f;     // 战力评分
    };

    BalanceReport validate(const CharacterStats& stats, const godot::Vector<FrameData>& frames) const;

private:
    static constexpr int32_t MIN_HP = 200;
    static constexpr int32_t MAX_HP = 800;
    static constexpr int32_t MIN_MP = 100;
    static constexpr int32_t MAX_MP = 800;
    static constexpr int32_t MIN_SPEED = 2;
    static constexpr int32_t MAX_SPEED = 8;
    static constexpr int32_t MIN_WEIGHT = 50;
    static constexpr int32_t MAX_WEIGHT = 200;
    static constexpr int32_t MAX_TOTAL_DAMAGE = 150; // 连段总伤害上限
};

} // namespace prank
