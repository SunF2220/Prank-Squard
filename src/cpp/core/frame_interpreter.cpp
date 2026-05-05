#include "frame_interpreter.h"

namespace prank {

void FrameInterpreter::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_frame_table", "frames"), &FrameInterpreter::set_frame_table);
    godot::ClassDB::bind_method(godot::D_METHOD("set_patterns", "patterns"), &FrameInterpreter::set_patterns);
    godot::ClassDB::bind_method(godot::D_METHOD("initialize", "fighter"), &FrameInterpreter::initialize);
    godot::ClassDB::bind_method(godot::D_METHOD("update", "fighter", "delta"), &FrameInterpreter::update);
}

FrameInterpreter::FrameInterpreter() {
}

void FrameInterpreter::set_frame_table(const godot::Vector<FrameData*>& frames) {
    frame_table_ = frames;
}

void FrameInterpreter::set_patterns(const godot::Vector<PatternData>& patterns) {
    patterns_ = patterns;
}

void FrameInterpreter::initialize(Fighter* fighter) {
    current_frame_index_ = 0;
    frame_timer_ = 0;
    if (frame_table_.size() > 0) {
        process_frame(fighter);
    }
}

void FrameInterpreter::update(Fighter* fighter, float delta) {
    if (frame_table_.size() == 0) return;

    const FrameData* current_frame = frame_table_[current_frame_index_];

    // 1. Apply velocity deltas
    fighter->apply_velocity(current_frame->dvx, current_frame->dvy, current_frame->dvz);

    // 2. Check input transitions
    int transition = check_input_transitions(fighter);
    if (transition >= 0 && transition < static_cast<int>(frame_table_.size())) {
        transition_to(fighter, transition);
        return;
    }

    // 3. Frame timer
    frame_timer_++;
    float frame_duration = 1.0f / 60.0f;
    int wait_frames = current_frame->wait > 0 ? current_frame->wait : 5;

    if (frame_timer_ >= wait_frames) {
        if (current_frame->is_return_to_idle()) {
            transition_to(fighter, 0); // Return to first frame (idle)
        } else {
            int next = current_frame->next;
            if (next >= 0 && next < static_cast<int>(frame_table_.size())) {
                transition_to(fighter, next);
            }
        }
    }

    // 4. Activate hitboxes
    fighter->set_active_itrs(current_frame->itrs, current_frame->itr_count);
    fighter->set_active_bdys(current_frame->bdys, current_frame->bdy_count);
}

int FrameInterpreter::check_input_transitions(Fighter* fighter) {
    InputBuffer* input = fighter->get_input_buffer();
    const FrameData* cf = frame_table_[current_frame_index_];

    // Detect special move patterns
    int special = input->detect_special(patterns_);
    if (special >= 0) return special;

    // Check direction + button jumps
    bool facing_left = fighter->get_facing() == -1;
    bool has_F = facing_left ? input->is_dir(InputSnapshot::LEFT) : input->is_dir(InputSnapshot::RIGHT);
    bool has_U = input->is_dir(InputSnapshot::UP);
    bool has_D = input->is_dir(InputSnapshot::DOWN);

    if (cf->hit_Fa >= 0 && has_F && input->attack_just_pressed()) return cf->hit_Fa;
    if (cf->hit_Ua >= 0 && has_U && input->attack_just_pressed()) return cf->hit_Ua;
    if (cf->hit_Da >= 0 && has_D && input->attack_just_pressed()) return cf->hit_Da;
    if (cf->hit_Fj >= 0 && has_F && input->jump_just_pressed())    return cf->hit_Fj;
    if (cf->hit_Uj >= 0 && has_U && input->jump_just_pressed())    return cf->hit_Uj;
    if (cf->hit_Dj >= 0 && has_D && input->jump_just_pressed())    return cf->hit_Dj;
    if (cf->hit_a >= 0 && input->attack_just_pressed())            return cf->hit_a;
    if (cf->hit_j >= 0 && input->jump_just_pressed())              return cf->hit_j;
    if (cf->hit_d >= 0 && input->defend_held())                    return cf->hit_d;

    return -1;
}

void FrameInterpreter::transition_to(Fighter* fighter, int new_index) {
    current_frame_index_ = new_index;
    frame_timer_ = 0;
    process_frame(fighter);
}

void FrameInterpreter::process_frame(Fighter* fighter) {
    const FrameData* frame = frame_table_[current_frame_index_];

    if (frame->mp != 0) {
        if (frame->mp > 0) {
            fighter->regenerate_mp();
        } else {
            fighter->consume_mp(-frame->mp);
        }
    }

    fighter->is_hurtable = frame->hurtable;
}

} // namespace prank
