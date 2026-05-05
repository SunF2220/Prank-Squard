#include "procedural_sprite.h"

namespace prank {

ProceduralSprite::ProceduralSprite() {
}

godot::Ref<godot::Image> ProceduralSprite::generate_sprite_sheet(
        const ArchetypeParams& archetype,
        const ElementParams& element,
        int frame_count) {

    int frame_w = 96;
    int frame_h = 128;
    int sheet_width = frame_w * frame_count;
    int sheet_height = frame_h;

    godot::Ref<godot::Image> sheet;
    sheet.instantiate();
    sheet->create(sheet_width, sheet_height, false, godot::Image::FORMAT_RGBA8);
    sheet->fill(godot::Color(0, 0, 0, 0)); // Transparent background

    for (int i = 0; i < frame_count; ++i) {
        draw_stick_figure(sheet, i * frame_w, 0, frame_w, frame_h, archetype, element, i);
    }

    return sheet;
}

void ProceduralSprite::draw_stick_figure(godot::Ref<godot::Image> img,
        int frame_x, int frame_y, int frame_width, int frame_height,
        const ArchetypeParams& arch, const ElementParams& elem, int frame_idx) {

    godot::Color outline(0.1f, 0.1f, 0.1f, 1.0f);
    godot::Color fill = arch.primary_color;
    godot::Color accent = elem.accent_color;

    int cx = frame_x + frame_width / 2;
    int base_y = frame_y + frame_height - 10;

    float h_scale = arch.height;
    float b_scale = arch.body_width;

    // Head
    int head_r = static_cast<int>(10 * arch.head_size);
    int head_y = base_y - static_cast<int>(95 * h_scale);
    draw_circle(img, cx, head_y, head_r, outline, fill);

    // Body (torso)
    int neck_y = head_y + head_r;
    int hip_y = base_y - static_cast<int>(35 * h_scale);
    int shoulder_width = static_cast<int>(10 * b_scale);
    draw_line(img, cx, neck_y, cx, hip_y, outline, 3);

    // Arms
    int elbow_y = neck_y + static_cast<int>(20 * h_scale);
    int hand_y = elbow_y + static_cast<int>(18 * h_scale);

    // Animation: sway for idle, punch for attack
    float swing = 0.0f;
    if (frame_idx == 5 || frame_idx == 6 || frame_idx == 7) {
        swing = 30.0f; // Attack reach
    } else if (frame_idx == 8) {
        swing = -15.0f; // Jump prep
    }

    float l_swing = swing;
    float r_swing = -swing * 0.3f;

    int l_shoulder_x = cx - shoulder_width;
    int r_shoulder_x = cx + shoulder_width;
    int l_elbow_x = l_shoulder_x - static_cast<int>(8 * b_scale + l_swing * 0.5f);
    int r_elbow_x = r_shoulder_x + static_cast<int>(8 * b_scale + r_swing * 0.5f);
    int l_hand_x = l_elbow_x - static_cast<int>(5 * b_scale + l_swing);
    int r_hand_x = r_elbow_x + static_cast<int>(5 * b_scale + r_swing);

    draw_line(img, l_shoulder_x, neck_y + 3, l_elbow_x, elbow_y, outline, 2);
    draw_line(img, l_elbow_x, elbow_y, l_hand_x, hand_y, outline, 2);
    draw_line(img, r_shoulder_x, neck_y + 3, r_elbow_x, elbow_y, outline, 2);
    draw_line(img, r_elbow_x, elbow_y, r_hand_x, hand_y, outline, 2);

    // Legs
    int knee_y = hip_y + static_cast<int>(22 * h_scale);
    int foot_y = base_y;

    float walk_phase = 0.0f;
    if (frame_idx >= 1 && frame_idx <= 3) {
        walk_phase = (frame_idx - 1) * 0.5f;
    }

    int l_knee_x = cx - 5 + static_cast<int>(godot::Math::sin(walk_phase * 3.14f) * 8);
    int r_knee_x = cx + 5 + static_cast<int>(godot::Math::sin(walk_phase * 3.14f + 3.14f) * 8);
    int l_foot_x = l_knee_x - 3;
    int r_foot_x = r_knee_x + 3;

    draw_line(img, cx - 5, hip_y, l_knee_x, knee_y, outline, 2);
    draw_line(img, l_knee_x, knee_y, l_foot_x, foot_y, outline, 2);
    draw_line(img, cx + 5, hip_y, r_knee_x, knee_y, outline, 2);
    draw_line(img, r_knee_x, knee_y, r_foot_x, foot_y, outline, 2);

    // Element aura (if any)
    if (elem.effect_type > 0 && (frame_idx == 5 || frame_idx == 6 || frame_idx == 7)) {
        for (int i = 0; i < 5; ++i) {
            int px = r_hand_x + (rand() % 20 - 10);
            int py = hand_y + (rand() % 20 - 10);
            draw_circle(img, px, py, 2, accent, accent);
        }
    }
}

void ProceduralSprite::draw_circle(godot::Ref<godot::Image> img,
        int cx, int cy, int r, const godot::Color& outline, const godot::Color& fill) {
    for (int y = -r; y <= r; ++y) {
        for (int x = -r; x <= r; ++x) {
            if (x * x + y * y <= r * r) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && py >= 0 && px < img->get_width() && py < img->get_height()) {
                    img->set_pixel(px, py, fill);
                }
            }
        }
    }
    // Outline
    for (int y = -r; y <= r; ++y) {
        for (int x = -r; x <= r; ++x) {
            int dist = x * x + y * y;
            if (dist <= r * r && dist >= (r - 1) * (r - 1)) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && py >= 0 && px < img->get_width() && py < img->get_height()) {
                    img->set_pixel(px, py, outline);
                }
            }
        }
    }
}

void ProceduralSprite::draw_line(godot::Ref<godot::Image> img,
        int x1, int y1, int x2, int y2, const godot::Color& color, int thickness) {
    int dx = godot::Math::abs(x2 - x1);
    int dy = godot::Math::abs(y2 - y1);
    int steps = dx > dy ? dx : dy;
    if (steps == 0) steps = 1;

    for (int i = 0; i <= steps; ++i) {
        float t = static_cast<float>(i) / steps;
        int px = x1 + static_cast<int>((x2 - x1) * t);
        int py = y1 + static_cast<int>((y2 - y1) * t);

        for (int ty = -thickness / 2; ty <= thickness / 2; ++ty) {
            for (int tx = -thickness / 2; tx <= thickness / 2; ++tx) {
                int sx = px + tx;
                int sy = py + ty;
                if (sx >= 0 && sy >= 0 && sx < img->get_width() && sy < img->get_height()) {
                    img->set_pixel(sx, sy, color);
                }
            }
        }
    }
}

void ProceduralSprite::draw_limb(godot::Ref<godot::Image> img,
        int x1, int y1, int x2, int y2, int joint_x, int joint_y,
        const godot::Color& color, int thickness) {
    draw_line(img, x1, y1, joint_x, joint_y, color, thickness);
    draw_line(img, joint_x, joint_y, x2, y2, color, thickness);
}

} // namespace prank
