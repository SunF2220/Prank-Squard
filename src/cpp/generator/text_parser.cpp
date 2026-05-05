#include "text_parser.h"

namespace prank {

TextParser::TextParser() {
    load_keywords();
}

void TextParser::load_keywords() {
    // 职业关键词
    struct { const char* kw; const char* val; } archetypes[] = {
        {"魔法师", "mage"}, {"法师", "mage"}, {"巫师", "mage"}, {"术士", "mage"},
        {"格斗家", "brawler"}, {"武术家", "brawler"}, {"拳师", "brawler"},
        {"忍者", "ninja"}, {"刺客", "ninja"},
        {"剑士", "swordsman"}, {"剑客", "swordsman"}, {"战士", "swordsman"},
        {"弓箭手", "archer"}, {"射手", "archer"}, {"猎人", "archer"},
        {"重装", "tank"}, {"坦克", "tank"}, {"骑士", "tank"},
        {nullptr, nullptr}
    };

    for (int i = 0; archetypes[i].kw; ++i) {
        keywords_.push_back({archetypes[i].kw, "archetype", archetypes[i].val, 10});
    }

    // 元素关键词
    struct { const char* kw; const char* val; } elements[] = {
        {"火", "fire"}, {"火焰", "fire"}, {"炎", "fire"},
        {"冰", "ice"}, {"冰冻", "ice"}, {"霜", "ice"}, {"寒", "ice"},
        {"雷", "thunder"}, {"电", "thunder"}, {"闪电", "thunder"},
        {"暗", "dark"}, {"黑暗", "dark"}, {"影", "dark"},
        {"光", "light"}, {"圣光", "light"}, {"光明", "light"},
        {"风", "wind"}, {"暴风", "wind"},
        {nullptr, nullptr}
    };

    for (int i = 0; elements[i].kw; ++i) {
        keywords_.push_back({elements[i].kw, "element", elements[i].val, 8});
    }

    // 武器关键词
    struct { const char* kw; const char* val; } weapons[] = {
        {"剑", "sword"}, {"刀", "blade"}, {"匕首", "dagger"},
        {"弓", "bow"}, {"弩", "crossbow"},
        {"杖", "staff"}, {"法杖", "staff"},
        {"拳", "fist"}, {"爪", "claw"},
        {"斧", "axe"}, {"锤", "hammer"},
        {"枪", "spear"}, {"矛", "spear"},
        {"手里剑", "shuriken"}, {"飞镖", "dart"},
        {nullptr, nullptr}
    };

    for (int i = 0; weapons[i].kw; ++i) {
        keywords_.push_back({weapons[i].kw, "weapon", weapons[i].val, 7});
    }

    // 特征关键词
    struct { const char* kw; const char* cat; } traits[] = {
        {"远程", "range_type"},
        {"近战", "range_type"},
        {"快速", "speed"},
        {"缓慢", "speed"},
        {"强大", "power"},
        {"灵活", "agility"},
        {"坚固", "defense"},
        {"治疗", "heal"},
        {"召唤", "summon"},
        {"瞬移", "teleport"},
        {nullptr, nullptr}
    };

    for (int i = 0; traits[i].kw; ++i) {
        keywords_.push_back({traits[i].kw, traits[i].cat, traits[i].kw, 3});
    }
}

ParsedIntent TextParser::parse(const godot::String& text) const {
    auto matches = match_keywords(text);
    return resolve_intent(matches);
}

godot::Dictionary TextParser::match_keywords(const godot::String& text) const {
    godot::Dictionary result;
    godot::String lower = text.to_lower();

    for (const auto& entry : keywords_) {
        if (lower.contains(entry.keyword)) {
            godot::Dictionary cat_dict = result.get(entry.category, godot::Dictionary());
            cat_dict[entry.value] = cat_dict.get(entry.value, godot::Variant(0)).operator int() + entry.weight;
            result[entry.category] = cat_dict;
        }
    }
    return result;
}

ParsedIntent TextParser::resolve_intent(const godot::Dictionary& matches) const {
    ParsedIntent intent;

    // Resolve archetype (highest weight)
    godot::Dictionary arch = matches.get("archetype", godot::Dictionary());
    int best_weight = 0;
    auto keys = arch.keys();
    for (int i = 0; i < keys.size(); ++i) {
        int w = arch.get(keys[i], godot::Variant(0)).operator int();
        if (w > best_weight) {
            best_weight = w;
            intent.archetype = keys[i];
        }
    }
    if (intent.archetype.is_empty()) {
        intent.archetype = "brawler"; // Default
    }

    // Resolve element
    godot::Dictionary elem = matches.get("element", godot::Dictionary());
    best_weight = 0;
    keys = elem.keys();
    for (int i = 0; i < keys.size(); ++i) {
        int w = elem.get(keys[i], godot::Variant(0)).operator int();
        if (w > best_weight) {
            best_weight = w;
            intent.element = keys[i];
        }
    }
    if (intent.element.is_empty()) {
        intent.element = "none";
    }

    // Resolve weapon
    godot::Dictionary weap = matches.get("weapon", godot::Dictionary());
    best_weight = 0;
    keys = weap.keys();
    for (int i = 0; i < keys.size(); ++i) {
        int w = weap.get(keys[i], godot::Variant(0)).operator int();
        if (w > best_weight) {
            best_weight = w;
            intent.weapon = keys[i];
        }
    }

    return intent;
}

} // namespace prank
