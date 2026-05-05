#include "ai_controller.h"
#include <cstdlib>

namespace prank {

AIController::AIController() {
    directions_ = 0;
    buttons_ = 0;
}

uint8_t AIController::get_inputs(int32_t ai_x, int32_t ai_y, int32_t ai_z,
                                  int32_t enemy_x, int32_t enemy_y, int32_t enemy_z,
                                  float delta) {
    decision_timer_ -= delta;
    if (decision_timer_ > 0.0f) {
        return directions_;
    }

    decision_timer_ = decision_interval_;

    int32_t dx = enemy_x - ai_x;
    int32_t dy = enemy_y - ai_y;
    int32_t dz = enemy_z - ai_z;

    directions_ = 0;
    buttons_ = 0;

    switch (difficulty_) {
        case EASY:   decide_easy(dx, dy, dz);   break;
        case NORMAL: decide_normal(dx, dy, dz); break;
        case HARD:   decide_hard(dx, dy, dz);   break;
    }

    return directions_;
}

void AIController::decide_easy(int32_t dx, int32_t dy, int32_t dz) {
    // Move toward enemy
    if (dx > 50)      directions_ |= InputSnapshot::RIGHT;
    else if (dx < -50) directions_ |= InputSnapshot::LEFT;

    if (dz > 30)      directions_ |= InputSnapshot::DOWN;
    else if (dz < -30) directions_ |= InputSnapshot::UP;

    // Randomly attack when close
    int abs_dx = dx > 0 ? dx : -dx;
    int abs_dz = dz > 0 ? dz : -dz;
    if (abs_dx < 80 && abs_dz < 40) {
        if (rand() % 100 < 40) {
            buttons_ |= InputSnapshot::ATTACK;
        }
    }
}

void AIController::decide_normal(int32_t dx, int32_t dy, int32_t dz) {
    int abs_dx = dx > 0 ? dx : -dx;
    int abs_dz = dz > 0 ? dz : -dz;

    // Move toward enemy with some spacing
    if (abs_dx > 60) {
        if (dx > 0) directions_ |= InputSnapshot::RIGHT;
        else        directions_ |= InputSnapshot::LEFT;
    }

    if (abs_dz > 20) {
        if (dz > 0) directions_ |= InputSnapshot::DOWN;
        else        directions_ |= InputSnapshot::UP;
    }

    // Attack when in range
    if (abs_dx < 70 && abs_dz < 30) {
        if (rand() % 100 < 60) {
            buttons_ |= InputSnapshot::ATTACK;
        }
    }

    // Defend if enemy is close and attacking
    if (abs_dx < 50) {
        if (rand() % 100 < 25) {
            buttons_ |= InputSnapshot::DEFEND;
        }
    }

    // Jump occasionally
    if (rand() % 100 < 5) {
        buttons_ |= InputSnapshot::JUMP;
    }
}

void AIController::decide_hard(int32_t dx, int32_t dy, int32_t dz) {
    int abs_dx = dx > 0 ? dx : -dx;
    int abs_dz = dz > 0 ? dz : -dz;

    // Optimize spacing
    int ideal_distance = 60;
    if (abs_dx > ideal_distance + 10) {
        if (dx > 0) directions_ |= InputSnapshot::RIGHT;
        else        directions_ |= InputSnapshot::LEFT;
    } else if (abs_dx < ideal_distance - 10 && abs_dx > 0) {
        if (dx > 0) directions_ |= InputSnapshot::LEFT;  // Back away
        else        directions_ |= InputSnapshot::RIGHT;
    }

    if (abs_dz > 20) {
        if (dz > 0) directions_ |= InputSnapshot::DOWN;
        else        directions_ |= InputSnapshot::UP;
    }

    // Combo attempts
    if (abs_dx < 80 && abs_dz < 30) {
        int r = rand() % 100;
        if (r < 50) {
            buttons_ |= InputSnapshot::ATTACK;
        } else if (r < 60) {
            buttons_ |= InputSnapshot::JUMP;
        } else if (r < 80) {
            buttons_ |= InputSnapshot::DEFEND;
        }
    }

    // Reactive defending
    if (abs_dx < 40) {
        if (rand() % 100 < 35) {
            buttons_ |= InputSnapshot::DEFEND;
        }
    }
}

} // namespace prank
