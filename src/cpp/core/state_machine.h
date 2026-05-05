#pragma once

#include "data_types.h"

namespace prank {

class Fighter;

// 状态机 - 固定数组, O(1) 切换
class FighterStateMachine {
public:
    static constexpr int MAX_STATES = 20;

    enum StateID : int16_t {
        STATE_IDLE = 0,
        STATE_WALK_FORWARD = 1,
        STATE_WALK_BACKWARD = 2,
        STATE_ATTACK = 3,
        STATE_ATTACK2 = 4,
        STATE_ATTACK3 = 5,
        STATE_JUMP = 6,
        STATE_DASH = 7,
        STATE_HURT = 8,
        STATE_KNOCKDOWN = 9,
        STATE_DEFEATED = 10,
        STATE_DEFEND = 11,
        STATE_SPECIAL = 12,
        STATE_SPECIAL2 = 13,
        STATE_SPECIAL3 = 14,
    };

    void set_fighter(Fighter* f) { fighter_ = f; }

    int get_current() const { return current_state_; }

    void enter(int state);
    void exit(int state);
    void update(float delta);
    void transition(int new_state);

private:
    Fighter* fighter_ = nullptr;
    int current_state_ = STATE_IDLE;
    int previous_state_ = STATE_IDLE;
    float state_timer_ = 0.0f;

    void update_idle(float delta);
    void update_walk(float delta);
    void update_attack(float delta);
    void update_jump(float delta);
    void update_hurt(float delta);
    void update_defeated(float delta);
    void update_defend(float delta);
    void update_special(float delta);
};

} // namespace prank
