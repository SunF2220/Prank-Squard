#include "template_matcher.h"

namespace prank {

TemplateMatcher::TemplateMatcher() {
    archetype_map_["brawler"] = get_brawler_template();
    archetype_map_["mage"] = get_mage_template();
    archetype_map_["ninja"] = get_ninja_template();
    archetype_map_["swordsman"] = get_swordsman_template();
    archetype_map_["archer"] = get_archer_template();
    archetype_map_["tank"] = get_tank_template();

    element_map_["fire"] = get_element_fire();
    element_map_["ice"] = get_element_ice();
    element_map_["thunder"] = get_element_thunder();
    element_map_["dark"] = get_element_dark();
    element_map_["light"] = get_element_light();
    element_map_["none"] = get_element_none();
}

ArchetypeParams TemplateMatcher::match_archetype(const ParsedIntent& intent) const {
    auto it = archetype_map_.find(intent.archetype);
    if (it) {
        return it->value;
    }
    return get_brawler_template();
}

ElementParams TemplateMatcher::match_element(const ParsedIntent& intent) const {
    auto it = element_map_.find(intent.element);
    if (it) {
        return it->value;
    }
    return get_element_none();
}

ArchetypeParams TemplateMatcher::apply_element_modifier(const ArchetypeParams& base, const ElementParams& elem) const {
    ArchetypeParams result = base;

    // Apply element color tinting
    result.primary_color = result.primary_color.lerp(elem.accent_color, 0.4f);
    result.secondary_color = result.secondary_color.lerp(elem.trail_color, 0.3f);

    // Modify stats based on element
    result.base_stats.max_hp = static_cast<int32_t>(result.base_stats.max_hp * elem.damage_modifier);
    result.base_stats.speed = static_cast<int32_t>(result.base_stats.speed * elem.speed_modifier);

    return result;
}

} // namespace prank
