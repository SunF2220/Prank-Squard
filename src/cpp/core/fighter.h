#pragma once

#include "data_types.h"
#include "input_buffer.h"
#include "state_machine.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

class FrameInterpreter;

class Fighter : public godot::Node {
    GDCLASS(Fighter, godot::Node);

public:
    Fighter();
    ~Fighter() = default;

    void set_frame_table(const godot::Vector<FrameData*>& frames);
    void set_patterns(const godot::Vector<PatternData>& patterns);
    void set_stats(const CharacterStats& stats);

    void apply_velocity(int16_t dvx, int16_t dvy, int16_t dvz);
    void set_active_itrs(const ItrData* itrs, int count);
    void set_active_bdys(const BdyData* bdys, int count);

    void change_state(int new_state);
    int get_current_state() const { return state_machine_.get_current(); }

    InputBuffer* get_input_buffer() { return &input_buffer_; }
    int get_sprite_frame() const { return current_sprite_frame_; }

    // Stats
    int32_t get_hp() const { return hp_; }
    int32_t get_mp() const { return mp_; }
    int32_t get_max_hp() const { return stats_.max_hp; }
    int32_t get_max_mp() const { return stats_.max_mp; }
    int32_t get_speed() const { return stats_.speed; }
    int32_t get_weight() const { return stats_.weight; }
    int32_t get_facing() const { return facing_; }

    void take_damage(int16_t damage, int16_t dvx, int16_t dvy, int16_t dvz);
    void consume_mp(int16_t amount);
    void regenerate_mp();

    // Position
    int32_t world_x = 0, world_y = 0, world_z = 0;
    int32_t vel_x = 0, vel_y = 0, vel_z = 0;
    int32_t facing_ = 1;

    bool is_grounded = true;
    bool is_blocking = false;
    bool is_hurtable = true;

    // Active hitboxes
    static constexpr int MAX_ACTIVE_ITRS = 4;
    static constexpr int MAX_ACTIVE_BDYS = 4;
    ItrData active_itrs_[MAX_ACTIVE_ITRS];
    BdyData active_bdys_[MAX_ACTIVE_BDYS];
    int active_itr_count_ = 0;
    int active_bdy_count_ = 0;

    int combo_count_ = 0;
    float combo_timer_ = 0.0f;

protected:
    static void _bind_methods();

private:
    InputBuffer input_buffer_;
    FighterStateMachine state_machine_;
    CharacterStats stats_;

    godot::Vector<FrameData*> frame_table_;
    godot::Vector<PatternData> patterns_;

    int32_t hp_ = 500;
    int32_t mp_ = 500;
    int current_sprite_frame_ = 0;

    friend class FrameInterpreter;
};

} // namespace prank
