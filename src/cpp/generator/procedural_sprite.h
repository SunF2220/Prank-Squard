#pragma once

#include "archetype_templates.h"
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace prank {

// 程序化火柴人精灵生成器
class ProceduralSprite {
public:
    ProceduralSprite();
    ~ProceduralSprite() = default;

    // 生成精灵表 (所有帧合并为一张 AtlasTexture)
    godot::Ref<godot::Image> generate_sprite_sheet(
        const ArchetypeParams& archetype,
        const ElementParams& element,
        int frame_count);

    // 绘制单帧火柴人
    void draw_stick_figure(godot::Ref<godot::Image> img, int frame_x, int frame_y,
                           int frame_width, int frame_height,
                           const ArchetypeParams& arch,
                           const ElementParams& elem,
                           int frame_idx);

private:
    void draw_circle(godot::Ref<godot::Image> img, int cx, int cy, int r,
                     const godot::Color& outline, const godot::Color& fill);
    void draw_line(godot::Ref<godot::Image> img,
                   int x1, int y1, int x2, int y2,
                   const godot::Color& color, int thickness = 3);
    void draw_limb(godot::Ref<godot::Image> img,
                   int x1, int y1, int x2, int y2, int joint_x, int joint_y,
                   const godot::Color& color, int thickness = 3);
};

} // namespace prank
