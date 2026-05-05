#pragma once

#include "fighter.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

class CombatSystem : public godot::Node {
    GDCLASS(CombatSystem, godot::Node);

public:
    CombatSystem();
    ~CombatSystem() = default;

    void register_fighter(Fighter* fighter);
    void unregister_fighter(Fighter* fighter);
    void update_hitboxes();
    void resolve_hits();

    void set_hitstop_frames(int frames) { hitstop_remaining_ = frames; }
    bool is_in_hitstop() const { return hitstop_remaining_ > 0; }
    void decrement_hitstop() { if (hitstop_remaining_ > 0) --hitstop_remaining_; }

private:
    godot::Vector<Fighter*> fighters_;
    int hitstop_remaining_ = 0;

    bool check_aabb_collision(const ItrData& itr, const BdyData& bdy,
                              int32_t atk_x, int32_t atk_y, int32_t atk_z,
                              int32_t def_x, int32_t def_y, int32_t def_z) const;

protected:
    static void _bind_methods();
};

} // namespace prank
