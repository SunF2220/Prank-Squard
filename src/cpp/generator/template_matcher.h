#pragma once

#include "text_parser.h"
#include "archetype_templates.h"
#include <godot_cpp/classes/ref_counted.hpp>

namespace prank {

class TemplateMatcher {
public:
    TemplateMatcher();
    ~TemplateMatcher() = default;

    ArchetypeParams match_archetype(const ParsedIntent& intent) const;
    ElementParams match_element(const ParsedIntent& intent) const;
    ArchetypeParams apply_element_modifier(const ArchetypeParams& base, const ElementParams& elem) const;

private:
    godot::HashMap<godot::String, ArchetypeParams> archetype_map_;
    godot::HashMap<godot::String, ElementParams> element_map_;
};

} // namespace prank
