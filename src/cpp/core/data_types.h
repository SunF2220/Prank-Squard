#pragma once

#include <cstdint>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace prank {

// ============================================================================
// 判定框数据结构 (POD, 紧凑内存布局)
// ============================================================================

struct ItrData {
    int16_t x = 0, y = 0, z = 0;
    int16_t width = 0, height = 0, depth = 0;
    int16_t damage = 0;
    int16_t dvx = 0, dvy = 0, dvz = 0;
    uint8_t effect_type = 0;    // 0=普通, 1=火, 2=冰, 3=雷, 4=暗, 5=光
    uint8_t fall = 0;
    bool blockable = true;
    bool ground_only = false;
    bool aerial_only = false;
};

struct BdyData {
    int16_t x = 0, y = 0, z = 0;
    int16_t width = 0, height = 0, depth = 0;
};

// ============================================================================
// 帧数据 - 单帧行为定义
// 大小 ≈ 192 字节, 内联数组避免堆分配
// ============================================================================

struct FrameData {
    static constexpr int MAX_ITRS = 4;
    static constexpr int MAX_BDYS = 4;

    int16_t pic = 0;            // 精灵帧索引
    int16_t state = 0;          // 状态ID
    int16_t wait = 0;           // 停留帧数
    int16_t next = 0;           // 跳转目标帧索引 (999 = 返回idle)
    int16_t dvx = 0, dvy = 0, dvz = 0;  // 速度增量
    int16_t mp = 0;             // 魔力变化
    int16_t shake = 0;          // 屏幕震动
    int32_t sound_id = 0;       // 音效ID

    // 输入跳转 (按键+方向 → 目标帧)
    int16_t hit_a = -1, hit_j = -1, hit_d = -1;
    int16_t hit_Fa = -1, hit_Ua = -1, hit_Da = -1;
    int16_t hit_Fj = -1, hit_Uj = -1, hit_Dj = -1;

    // 判定框 (内联数组)
    ItrData itrs[MAX_ITRS];
    BdyData bdys[MAX_BDYS];
    uint8_t itr_count = 0;
    uint8_t bdy_count = 0;

    // 对象生成 (opoint)
    int16_t opoint_id = -1;
    int16_t opoint_x = 0, opoint_y = 0, opoint_z = 0;
    int16_t opoint_dvx = 0, opoint_dvy = 0, opoint_dvz = 0;

    bool hurtable = true;

    bool is_return_to_idle() const { return next == 999; }
};

// ============================================================================
// 输入快照 (环形缓冲区元素)
// ============================================================================

struct InputSnapshot {
    static constexpr uint8_t UP    = 1 << 0;
    static constexpr uint8_t DOWN  = 1 << 1;
    static constexpr uint8_t LEFT  = 1 << 2;
    static constexpr uint8_t RIGHT = 1 << 3;
    static constexpr uint8_t ATTACK = 1 << 4;
    static constexpr uint8_t JUMP  = 1 << 5;
    static constexpr uint8_t DEFEND = 1 << 6;

    uint8_t directions = 0;
    uint8_t buttons = 0;
    uint8_t just_pressed = 0;
    int16_t facing = 1;  // 1=右, -1=左
};

// ============================================================================
// 必杀技指令模式
// ============================================================================

struct PatternData {
    static constexpr int MAX_SEQUENCE = 8;
    uint8_t sequence[MAX_SEQUENCE];  // 方向序列
    uint8_t length = 0;
    uint8_t required_button = 0;     // 需要按下的按键
    int16_t target_frame = 0;        // 匹配成功后跳转的帧
    int16_t mp_cost = 0;
    bool ends_with_button = true;
};

// ============================================================================
// 2.5D 世界坐标
// ============================================================================

struct WorldPosition {
    int32_t x = 0;   // 水平轴 (-2000 ~ 2000)
    int32_t y = 0;   // 跳跃高度 (-500 ~ 500)
    int32_t z = 0;   // 纵深 (-200 ~ 200)

    godot::Vector2 to_screen(float z_scale = 0.5f) const {
        return godot::Vector2(static_cast<float>(x), static_cast<float>(z) * z_scale - static_cast<float>(y));
    }

    int32_t get_sort_order() const { return -z; }
};

// ============================================================================
// 角色属性数据
// ============================================================================

struct CharacterStats {
    int32_t max_hp = 500;
    int32_t max_mp = 500;
    int32_t mp_regen = 3;       // 每帧回复 (60fps)
    int32_t speed = 5;
    int32_t jump_power = 12;
    int32_t weight = 100;       // 影响受击击退
};

// ============================================================================
// 招式定义
// ============================================================================

struct MoveDef {
    godot::String name;
    int16_t start_frame = 0;    // 帧表中的起始索引
    int16_t mp_cost = 0;
    godot::String input_sequence; // e.g. "D,DR,R,A"
};

} // namespace prank
