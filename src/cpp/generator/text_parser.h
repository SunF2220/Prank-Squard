#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

// 解析后的意图
struct ParsedIntent {
    godot::String archetype;    // 职业: "mage", "brawler", "ninja", etc.
    godot::String element;      // 元素: "fire", "ice", "thunder", etc.
    godot::String weapon;       // 武器: "sword", "bow", "staff", etc.
    godot::String gender;       // 性别
    godot::String personality;  // 性格特征
    godot::String extra;        // 额外描述
    float confidence = 0.0f;    // 匹配置信度
};

// 文本解析器 - 中文关键词提取 + 模板匹配
class TextParser {
public:
    TextParser();
    ~TextParser() = default;

    ParsedIntent parse(const godot::String& text) const;

private:
    struct KeywordEntry {
        godot::String keyword;
        godot::String category;
        godot::String value;
        int weight = 1;
    };

    godot::Vector<KeywordEntry> keywords_;

    void load_keywords();
    godot::String segment_word(const godot::String& text) const;
    godot::Dictionary match_keywords(const godot::String& text) const;
    ParsedIntent resolve_intent(const godot::Dictionary& matches) const;
};

} // namespace prank
