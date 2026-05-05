#include "fighter.h"

namespace prank {

void Fighter::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_frame_table", "frames"), &Fighter::set_frame_table);
    godot::ClassDB::bind_method(godot::D_METHOD("set_patterns", "patterns"), &Fighter::set_patterns);
    godot::ClassDB::bind_method(godot::D_METHOD("set_stats", "stats"), &Fighter::set_stats);
    godot::ClassDB::bind_method(godot::D_METHOD("get_hp"), &Fighter::get_hp);
    godot::ClassDB::bind_method(godot::D_METHOD("get_mp"), &Fighter::get_mp);
    godot::ClassDB::bind_method(godot::D_METHOD("get_max_hp"), &Fighter::get_max_hp);
    godot::ClassDB::bind_method(godot::D_METHOD("get_max_mp"), &Fighter::get_max_mp);
    godot::ClassDB::bind_method(godot::D_METHOD("get_speed"), &Fighter::get_speed);
    godot::ClassDB::bind_method(godot::D_METHOD("get_weight"), &Fighter::get_weight);
    godot::ClassDB::bind_method(godot::D_METHOD("get_facing"), &Fighter::get_facing);
    godot::ClassDB::bind_method(godot::D_METHOD("take_damage", "damage", "dvx", "dvy", "dvz"), &Fighter::take_damage);
    godot::ClassDB::bind_method(godot::D_METHOD("consume_mp", "amount"), &Fighter::consume_mp);
    godot::ClassDB::bind_method(godot::D_METHOD("get_current_state"), &Fighter::get_current_state);
    godot::ClassDB::bind_method(godot::D_METHOD("change_state", "new_state"), &Fighter::change_state);
    godot::ClassDB::bind_method(godot::D_METHOD("get_sprite_frame"), &Fighter::get_sprite_frame);
}

Fighter::Fighter() {
    state_machine_.set_fighter(this);
    stats_ = CharacterStats{};
    hp_ = stats_.max_hp;
    mp_ = stats_.max_mp;
}

void Fighter::set_frame_table(const godot::Vector<FrameData*>& frames) {
    frame_table_ = frames;
}

void Fighter::set_patterns(const godot::Vector<PatternData>& patterns) {
    patterns_ = patterns;
}

void Fighter::set_stats(const CharacterStats& stats) {
    stats_ = stats;
    hp_ = stats.max_hp;
    mp_ = stats.max_mp;
}

void Fighter::apply_velocity(int16_t dvx, int16_t dvy, int16_t dvz) {
    vel_x += dvx * facing_;
    vel_y += dvy;
    vel_z += dvz;
}

void Fighter::set_active_itrs(const ItrData* itrs, int count) {
    active_itr_count_ = count < MAX_ACTIVE_ITRS ? count : MAX_ACTIVE_ITRS;
    for (int i = 0; i < active_itr_count_; ++i) {
        active_itrs_[i] = itrs[i];
    }
}

void Fighter::set_active_bdys(const BdyData* bdys, int count) {
    active_bdy_count_ = count < MAX_ACTIVE_BDYS ? count : MAX_ACTIVE_BDYS;
    for (int i = 0; i < active_bdy_count_; ++i) {
        active_bdys_[i] = bdys[i];
    }
}

void Fighter::change_state(int new_state) {
    state_machine_.transition(new_state);
}

void Fighter::take_damage(int16_t damage, int16_t dvx, int16_t dvy, int16_t dvz) {
    if (!is_hurtable) return;

    if (is_blocking) {
        damage = damage / 3;
        dvx /= 2;
        dvy /= 2;
    }

    hp_ -= damage;
    vel_x += dvx;
    vel_y += dvy;
    vel_z += dvz;

    if (hp_ <= 0) {
        hp_ = 0;
        change_state(10); // defeated state
    } else if (damage > 0) {
        int weight_factor = stats_.weight / 50;
        vel_x /= weight_factor > 0 ? weight_factor : 1;
        change_state(8); // hurt state
    }
}

void Fighter::consume_mp(int16_t amount) {
    mp_ -= amount;
    if (mp_ < 0) mp_ = 0;
}

void Fighter::regenerate_mp() {
    mp_ += stats_.mp_regen;
    if (mp_ > stats_.max_mp) mp_ = stats_.max_mp;
}

} // namespace prank
