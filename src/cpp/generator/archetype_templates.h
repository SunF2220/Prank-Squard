#pragma once

#include "../core/data_types.h"
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/color.hpp>

namespace prank {

// 职业模板参数
struct ArchetypeParams {
    godot::String name;
    CharacterStats base_stats;
    godot::Color primary_color;
    godot::Color secondary_color;

    // 体型参数 (用于程序化精灵)
    float head_size = 1.0f;
    float body_width = 1.0f;
    float limb_length = 1.0f;
    float height = 1.0f;

    // 默认招式模板名称
    godot::String basic_attack;
    godot::String special1;
    godot::String special2;
    godot::String ultimate;
};

// 元素参数
struct ElementParams {
    godot::String name;
    int16_t effect_type;     // 对应 ItrData::effect_type
    godot::Color accent_color;
    godot::Color trail_color;
    float damage_modifier = 1.0f;
    float speed_modifier = 1.0f;
    godot::String status_effect; // "" / "burn" / "freeze" / "paralyze"
};

// 预设职业模板
inline ArchetypeParams get_brawler_template() {
    ArchetypeParams t;
    t.name = "Brawler";
    t.base_stats = {500, 400, 2, 5, 12, 100};
    t.primary_color = godot::Color(0.9f, 0.3f, 0.1f);
    t.secondary_color = godot::Color(0.2f, 0.1f, 0.05f);
    t.head_size = 1.0f;
    t.body_width = 1.1f;
    t.limb_length = 1.0f;
    t.height = 1.0f;
    t.basic_attack = "combo_punch";
    t.special1 = "uppercut";
    t.special2 = "dash_punch";
    t.ultimate = "flurry";
    return t;
}

inline ArchetypeParams get_mage_template() {
    ArchetypeParams t;
    t.name = "Mage";
    t.base_stats = {350, 700, 5, 4, 10, 80};
    t.primary_color = godot::Color(0.2f, 0.1f, 0.8f);
    t.secondary_color = godot::Color(0.1f, 0.05f, 0.4f);
    t.head_size = 1.1f;
    t.body_width = 0.85f;
    t.limb_length = 1.1f;
    t.height = 1.05f;
    t.basic_attack = "magic_bolt";
    t.special1 = "elemental_blast";
    t.special2 = "teleport";
    t.ultimate = "cataclysm";
    return t;
}

inline ArchetypeParams get_ninja_template() {
    ArchetypeParams t;
    t.name = "Ninja";
    t.base_stats = {380, 450, 4, 7, 14, 70};
    t.primary_color = godot::Color(0.1f, 0.1f, 0.1f);
    t.secondary_color = godot::Color(0.3f, 0.05f, 0.5f);
    t.head_size = 0.85f;
    t.body_width = 0.7f;
    t.limb_length = 1.15f;
    t.height = 0.95f;
    t.basic_attack = "quick_strike";
    t.special1 = "shuriken_throw";
    t.special2 = "shadow_dash";
    t.ultimate = "assassinate";
    return t;
}

inline ArchetypeParams get_swordsman_template() {
    ArchetypeParams t;
    t.name = "Swordsman";
    t.base_stats = {450, 400, 3, 5, 11, 95};
    t.primary_color = godot::Color(0.8f, 0.6f, 0.2f);
    t.secondary_color = godot::Color(0.5f, 0.3f, 0.1f);
    t.head_size = 1.0f;
    t.body_width = 1.0f;
    t.limb_length = 1.0f;
    t.height = 1.0f;
    t.basic_attack = "sword_slash";
    t.special1 = "blade_wave";
    t.special2 = "thrust";
    t.ultimate = "whirlwind";
    return t;
}

inline ArchetypeParams get_archer_template() {
    ArchetypeParams t;
    t.name = "Archer";
    t.base_stats = {380, 450, 3, 5, 11, 85};
    t.primary_color = godot::Color(0.1f, 0.5f, 0.1f);
    t.secondary_color = godot::Color(0.05f, 0.3f, 0.05f);
    t.head_size = 0.95f;
    t.body_width = 0.8f;
    t.limb_length = 1.05f;
    t.height = 1.0f;
    t.basic_attack = "rapid_shot";
    t.special1 = "piercing_arrow";
    t.special2 = "multi_shot";
    t.ultimate = "arrow_rain";
    return t;
}

inline ArchetypeParams get_tank_template() {
    ArchetypeParams t;
    t.name = "Tank";
    t.base_stats = {700, 250, 1, 3, 7, 150};
    t.primary_color = godot::Color(0.5f, 0.5f, 0.5f);
    t.secondary_color = godot::Color(0.3f, 0.3f, 0.3f);
    t.head_size = 1.15f;
    t.body_width = 1.5f;
    t.limb_length = 0.85f;
    t.height = 1.1f;
    t.basic_attack = "heavy_punch";
    t.special1 = "ground_slam";
    t.special2 = "charge";
    t.ultimate = "earthquake";
    return t;
}

// 元素参数表
inline ElementParams get_element_fire() {
    return {"Fire", 1, godot::Color(1.0f, 0.3f, 0.0f), godot::Color(1.0f, 0.5f, 0.1f), 1.2f, 1.0f, "burn"};
}

inline ElementParams get_element_ice() {
    return {"Ice", 2, godot::Color(0.3f, 0.8f, 1.0f), godot::Color(0.6f, 0.9f, 1.0f), 0.9f, 0.8f, "freeze"};
}

inline ElementParams get_element_thunder() {
    return {"Thunder", 3, godot::Color(1.0f, 0.8f, 0.0f), godot::Color(1.0f, 1.0f, 0.3f), 1.1f, 1.3f, "paralyze"};
}

inline ElementParams get_element_dark() {
    return {"Dark", 4, godot::Color(0.3f, 0.0f, 0.5f), godot::Color(0.1f, 0.0f, 0.2f), 1.3f, 0.9f, "drain"};
}

inline ElementParams get_element_light() {
    return {"Light", 5, godot::Color(1.0f, 1.0f, 0.5f), godot::Color(1.0f, 1.0f, 0.8f), 0.9f, 1.1f, "blind"};
}

inline ElementParams get_element_none() {
    return {"None", 0, godot::Color(1.0f, 1.0f, 1.0f), godot::Color(0.8f, 0.8f, 0.8f), 1.0f, 1.0f, ""};
}

} // namespace prank
