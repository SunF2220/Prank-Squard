#include "combat_system.h"

namespace prank {

void CombatSystem::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("register_fighter", "fighter"), &CombatSystem::register_fighter);
    godot::ClassDB::bind_method(godot::D_METHOD("unregister_fighter", "fighter"), &CombatSystem::unregister_fighter);
    godot::ClassDB::bind_method(godot::D_METHOD("update_hitboxes"), &CombatSystem::update_hitboxes);
    godot::ClassDB::bind_method(godot::D_METHOD("resolve_hits"), &CombatSystem::resolve_hits);
    godot::ClassDB::bind_method(godot::D_METHOD("is_in_hitstop"), &CombatSystem::is_in_hitstop);
}

CombatSystem::CombatSystem() {
}

void CombatSystem::register_fighter(Fighter* fighter) {
    fighters_.push_back(fighter);
}

void CombatSystem::unregister_fighter(Fighter* fighter) {
    int idx = fighters_.find(fighter);
    if (idx >= 0) {
        fighters_.remove_at(idx);
    }
}

void CombatSystem::update_hitboxes() {
    if (is_in_hitstop()) {
        decrement_hitstop();
        return;
    }

    int count = fighters_.size();
    for (int ai = 0; ai < count; ++ai) {
        Fighter* attacker = fighters_[ai];
        if (!attacker || attacker->get_hp() <= 0) continue;

        for (int itr_i = 0; itr_i < attacker->active_itr_count_; ++itr_i) {
            const ItrData& itr = attacker->active_itrs_[itr_i];

            for (int di = 0; di < count; ++di) {
                if (ai == di) continue;
                Fighter* defender = fighters_[di];
                if (!defender || defender->get_hp() <= 0) continue;

                for (int bdy_i = 0; bdy_i < defender->active_bdy_count_; ++bdy_i) {
                    const BdyData& bdy = defender->active_bdys_[bdy_i];

                    if (check_aabb_collision(
                            itr, bdy,
                            attacker->world_x, attacker->world_y, attacker->world_z,
                            defender->world_x, defender->world_y, defender->world_z)) {

                        if (itr.ground_only && !defender->is_grounded) continue;
                        if (itr.aerial_only && defender->is_grounded) continue;

                        defender->take_damage(
                            itr.damage,
                            itr.dvx * attacker->facing_,
                            itr.dvy,
                            itr.dvz
                        );

                        set_hitstop_frames(6);
                        attacker->combo_count_++;
                        attacker->combo_timer_ = 60.0f;
                        return; // One hit per frame
                    }
                }
            }
        }
    }
}

void CombatSystem::resolve_hits() {
    // Apply physics to all fighters
    for (Fighter* f : fighters_) {
        if (!f) continue;

        // Apply velocities
        f->world_x += f->vel_x;
        f->world_y += f->vel_y;
        f->world_z += f->vel_z;

        // Clamp
        if (f->world_x < -1000) f->world_x = -1000;
        if (f->world_x > 1000)  f->world_x = 1000;
        if (f->world_y < 0)     f->world_y = 0;
        if (f->world_z < -200)  f->world_z = -200;
        if (f->world_z > 200)   f->world_z = 200;

        // Friction
        if (f->is_grounded) {
            f->vel_x = f->vel_x * 8 / 10;
            f->vel_z = f->vel_z * 8 / 10;
        }
    }
}

bool CombatSystem::check_aabb_collision(
        const ItrData& itr, const BdyData& bdy,
        int32_t atk_x, int32_t atk_y, int32_t atk_z,
        int32_t def_x, int32_t def_y, int32_t def_z) const {

    int32_t itr_x = atk_x + itr.x;
    int32_t itr_y = atk_y + itr.y;
    int32_t itr_z = atk_z + itr.z;
    int32_t itr_x2 = itr_x + itr.width;
    int32_t itr_y2 = itr_y + itr.height;
    int32_t itr_z2 = itr_z + itr.depth;

    int32_t bdy_x = def_x + bdy.x;
    int32_t bdy_y = def_y + bdy.y;
    int32_t bdy_z = def_z + bdy.z;
    int32_t bdy_x2 = bdy_x + bdy.width;
    int32_t bdy_y2 = bdy_y + bdy.height;
    int32_t bdy_z2 = bdy_z + bdy.depth;

    return (itr_x < bdy_x2 && itr_x2 > bdy_x &&
            itr_y < bdy_y2 && itr_y2 > bdy_y &&
            itr_z < bdy_z2 && itr_z2 > bdy_z);
}

} // namespace prank
