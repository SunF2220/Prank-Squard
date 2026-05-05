#include "state_machine.h"
#include "fighter.h"

namespace prank {

void FighterStateMachine::transition(int new_state) {
    if (new_state == current_state_) return;
    exit(current_state_);
    previous_state_ = current_state_;
    current_state_ = new_state;
    state_timer_ = 0.0f;
    enter(new_state);
}

void FighterStateMachine::enter(int state) {
    switch (state) {
        case STATE_JUMP:
            if (fighter_->is_grounded) {
                fighter_->vel_y = fighter_->stats_.jump_power;
                fighter_->is_grounded = false;
            }
            break;
        case STATE_DEFEND:
            fighter_->is_blocking = true;
            break;
        default:
            break;
    }
}

void FighterStateMachine::exit(int state) {
    switch (state) {
        case STATE_DEFEND:
            fighter_->is_blocking = false;
            break;
        case STATE_ATTACK:
        case STATE_ATTACK2:
        case STATE_ATTACK3:
        case STATE_SPECIAL:
        case STATE_SPECIAL2:
        case STATE_SPECIAL3:
            fighter_->active_itr_count_ = 0;
            fighter_->active_bdy_count_ = 0;
            break;
        default:
            break;
    }
}

void FighterStateMachine::update(float delta) {
    state_timer_ += delta;

    switch (current_state_) {
        case STATE_IDLE:        update_idle(delta);     break;
        case STATE_WALK_FORWARD:
        case STATE_WALK_BACKWARD: update_walk(delta);   break;
        case STATE_ATTACK:
        case STATE_ATTACK2:
        case STATE_ATTACK3:     update_attack(delta);   break;
        case STATE_JUMP:        update_jump(delta);     break;
        case STATE_HURT:
        case STATE_KNOCKDOWN:   update_hurt(delta);     break;
        case STATE_DEFEATED:    update_defeated(delta); break;
        case STATE_DEFEND:      update_defend(delta);   break;
        case STATE_SPECIAL:
        case STATE_SPECIAL2:
        case STATE_SPECIAL3:    update_special(delta);  break;
        default: break;
    }
}

void FighterStateMachine::update_idle(float delta) {
    auto* input = fighter_->get_input_buffer();
    const auto& snap = input->latest();

    if (snap.buttons & InputSnapshot::JUMP && fighter_->is_grounded) {
        transition(STATE_JUMP);
        return;
    }
    if (snap.buttons & InputSnapshot::ATTACK) {
        transition(STATE_ATTACK);
        return;
    }
    if (snap.buttons & InputSnapshot::DEFEND) {
        transition(STATE_DEFEND);
        return;
    }
    if (snap.directions & (InputSnapshot::LEFT | InputSnapshot::RIGHT)) {
        transition(STATE_WALK_FORWARD);
        return;
    }
}

void FighterStateMachine::update_walk(float delta) {
    auto* input = fighter_->get_input_buffer();

    fighter_->vel_x = fighter_->stats_.speed * fighter_->facing_;

    if (input->jump_just_pressed() && fighter_->is_grounded) {
        transition(STATE_JUMP);
        return;
    }
    if (input->attack_just_pressed()) {
        fighter_->vel_x = 0;
        transition(STATE_ATTACK);
        return;
    }
    if (!(input->latest().directions & (InputSnapshot::LEFT | InputSnapshot::RIGHT))) {
        fighter_->vel_x = 0;
        transition(STATE_IDLE);
        return;
    }
}

void FighterStateMachine::update_attack(float delta) {
    // Simple timed attack - return to idle after a fixed duration
    if (state_timer_ > 0.4f) {
        transition(STATE_IDLE);
    }
}

void FighterStateMachine::update_jump(float delta) {
    auto* input = fighter_->get_input_buffer();

    fighter_->vel_y -= 1; // gravity

    if (input->attack_just_pressed()) {
        // Aerial attack
    }

    if (fighter_->world_y <= 0 && fighter_->vel_y <= 0) {
        fighter_->world_y = 0;
        fighter_->vel_y = 0;
        fighter_->is_grounded = true;
        transition(STATE_IDLE);
    }
}

void FighterStateMachine::update_hurt(float delta) {
    if (state_timer_ > 0.3f) {
        if (fighter_->world_y <= 0) {
            fighter_->world_y = 0;
            fighter_->is_grounded = true;
            transition(STATE_IDLE);
        }
    }
}

void FighterStateMachine::update_defeated(float delta) {
    // Stay defeated
}

void FighterStateMachine::update_defend(float delta) {
    auto* input = fighter_->get_input_buffer();
    if (!(input->latest().buttons & InputSnapshot::DEFEND)) {
        transition(STATE_IDLE);
        return;
    }
}

void FighterStateMachine::update_special(float delta) {
    if (state_timer_ > 0.6f) {
        transition(STATE_IDLE);
    }
}

} // namespace prank
