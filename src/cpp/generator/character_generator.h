#pragma once

#include "text_parser.h"
#include "template_matcher.h"
#include "../core/data_types.h"
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/resource.hpp>

namespace prank {

class CharacterGenerator : public godot::Resource {
    GDCLASS(CharacterGenerator, godot::Resource);

public:
    CharacterGenerator();
    ~CharacterGenerator() = default;

    godot::Dictionary generate_from_text(const godot::String& description) const;
    godot::Dictionary generate_preview(const godot::String& description) const;

private:
    TextParser parser_;
    TemplateMatcher matcher_;

    godot::Vector<FrameData> generate_frame_data(const ArchetypeParams& arch, const ElementParams& elem) const;
    godot::Dictionary generate_move_list(const ArchetypeParams& arch) const;

protected:
    static void _bind_methods();
};

} // namespace prank
