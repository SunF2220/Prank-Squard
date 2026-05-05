#include "character_generator.h"

namespace prank {

void CharacterGenerator::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("generate_from_text", "description"), &CharacterGenerator::generate_from_text);
    godot::ClassDB::bind_method(godot::D_METHOD("generate_preview", "description"), &CharacterGenerator::generate_preview);
}

CharacterGenerator::CharacterGenerator() {
}

godot::Dictionary CharacterGenerator::generate_from_text(const godot::String& description) const {
    godot::Dictionary result;

    ParsedIntent intent = parser_.parse(description);
    ArchetypeParams arch = matcher_.match_archetype(intent);
    ElementParams elem = matcher_.match_element(intent);
    ArchetypeParams final_arch = matcher_.apply_element_modifier(arch, elem);

    result["character_id"] = final_arch.name.to_lower().replace(" ", "_") + "_" + elem.name.to_lower();
    result["display_name"] = elem.name + " " + final_arch.name;
    result["archetype"] = final_arch.name;
    result["element"] = elem.name;
    result["max_hp"] = final_arch.base_stats.max_hp;
    result["max_mp"] = final_arch.base_stats.max_mp;
    result["mp_regen"] = final_arch.base_stats.mp_regen;
    result["speed"] = final_arch.base_stats.speed;
    result["jump_power"] = final_arch.base_stats.jump_power;
    result["weight"] = final_arch.base_stats.weight;
    result["primary_color"] = final_arch.primary_color;
    result["secondary_color"] = final_arch.secondary_color;
    result["accent_color"] = elem.accent_color;
    result["effect_type"] = elem.effect_type;
    result["status_effect"] = elem.status_effect;
    result["moves"] = generate_move_list(final_arch);

    return result;
}

godot::Dictionary CharacterGenerator::generate_preview(const godot::String& description) const {
    return generate_from_text(description);
}

godot::Dictionary CharacterGenerator::generate_move_list(const ArchetypeParams& arch) const {
    godot::Dictionary moves;

    godot::Dictionary basic;
    basic["name"] = arch.basic_attack;
    basic["mp_cost"] = 0;
    basic["input"] = "A";
    moves["basic"] = basic;

    godot::Dictionary sp1;
    sp1["name"] = arch.special1;
    sp1["mp_cost"] = 100;
    sp1["input"] = "D,DR,R,A";
    moves["special1"] = sp1;

    godot::Dictionary sp2;
    sp2["name"] = arch.special2;
    sp2["mp_cost"] = 150;
    sp2["input"] = "R,DR,D,A";
    moves["special2"] = sp2;

    godot::Dictionary ult;
    ult["name"] = arch.ultimate;
    ult["mp_cost"] = 300;
    ult["input"] = "D,U,A";
    moves["ultimate"] = ult;

    return moves;
}

godot::Vector<FrameData> CharacterGenerator::generate_frame_data(const ArchetypeParams& arch, const ElementParams& elem) const {
    godot::Vector<FrameData> frames;

    // Frame 0: Idle
    FrameData idle;
    idle.pic = 0;
    idle.state = 0;
    idle.wait = 5;
    idle.next = 0;
    idle.hurtable = true;
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        idle.bdys[0] = bdy;
        idle.bdy_count = 1;
    }
    frames.push_back(idle);

    // Frame 1-3: Walk cycle
    for (int i = 0; i < 3; ++i) {
        FrameData walk;
        walk.pic = 1 + i;
        walk.state = 1;
        walk.wait = 4;
        walk.next = (i + 1) % 3 + 1;
        walk.dvx = arch.base_stats.speed;
        walk.hurtable = true;
        walk.hit_a = 5; // Can transition to attack
        walk.hit_j = 8; // Can transition to jump
        {
            BdyData bdy;
            bdy.x = -15; bdy.y = -80; bdy.z = -10;
            bdy.width = 30; bdy.height = 80; bdy.depth = 20;
            walk.bdys[0] = bdy;
            walk.bdy_count = 1;
        }
        frames.push_back(walk);
    }

    // Frame 5-7: Basic attack combo
    FrameData attack1;
    attack1.pic = 5;
    attack1.state = 3;
    attack1.wait = 3;
    attack1.next = 6;
    attack1.hurtable = true;
    attack1.hit_a = 6; // Combo chain
    {
        ItrData itr;
        itr.x = 30; itr.y = -10; itr.z = 0;
        itr.width = 35; itr.height = 20; itr.depth = 15;
        itr.damage = 15;
        itr.dvx = 10; itr.dvz = 0;
        itr.effect_type = elem.effect_type;
        itr.fall = 0;
        attack1.itrs[0] = itr;
        attack1.itr_count = 1;
    }
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        attack1.bdys[0] = bdy;
        attack1.bdy_count = 1;
    }
    frames.push_back(attack1);

    FrameData attack2;
    attack2.pic = 6;
    attack2.state = 3;
    attack2.wait = 3;
    attack2.next = 7;
    attack2.hurtable = true;
    attack2.hit_a = 7;
    {
        ItrData itr;
        itr.x = 30; itr.y = -5; itr.z = 5;
        itr.width = 35; itr.height = 25; itr.depth = 15;
        itr.damage = 20;
        itr.dvx = 12; itr.dvz = 5;
        itr.effect_type = elem.effect_type;
        itr.fall = 0;
        attack2.itrs[0] = itr;
        attack2.itr_count = 1;
    }
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        attack2.bdys[0] = bdy;
        attack2.bdy_count = 1;
    }
    frames.push_back(attack2);

    FrameData attack3;
    attack3.pic = 7;
    attack3.state = 3;
    attack3.wait = 5;
    attack3.next = 999; // Return to idle
    attack3.hurtable = true;
    {
        ItrData itr;
        itr.x = 25; itr.y = -20; itr.z = 0;
        itr.width = 40; itr.height = 30; itr.depth = 20;
        itr.damage = 30;
        itr.dvx = 18; itr.dvy = 5; itr.dvz = 0;
        itr.effect_type = elem.effect_type;
        itr.fall = 1;
        attack3.itrs[0] = itr;
        attack3.itr_count = 1;
    }
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        attack3.bdys[0] = bdy;
        attack3.bdy_count = 1;
    }
    frames.push_back(attack3);

    // Frame 8-10: Jump
    FrameData jump;
    jump.pic = 8;
    jump.state = 6;
    jump.wait = 2;
    jump.next = 9;
    jump.dvy = arch.base_stats.jump_power;
    jump.hurtable = true;
    jump.hit_a = 10;
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        jump.bdys[0] = bdy;
        jump.bdy_count = 1;
    }
    frames.push_back(jump);

    FrameData jump_air;
    jump_air.pic = 9;
    jump_air.state = 6;
    jump_air.wait = 10;
    jump_air.next = 999;
    jump_air.hurtable = true;
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        jump_air.bdys[0] = bdy;
        jump_air.bdy_count = 1;
    }
    frames.push_back(jump_air);

    // Frame 10: Aerial attack
    FrameData air_attack;
    air_attack.pic = 10;
    air_attack.state = 6;
    air_attack.wait = 5;
    air_attack.next = 999;
    air_attack.hurtable = true;
    {
        ItrData itr;
        itr.x = 20; itr.y = -10; itr.z = 0;
        itr.width = 30; itr.height = 25; itr.depth = 15;
        itr.damage = 20;
        itr.dvx = 8; itr.dvy = -5;
        itr.effect_type = elem.effect_type;
        itr.aerial_only = true;
        air_attack.itrs[0] = itr;
        air_attack.itr_count = 1;
    }
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        air_attack.bdys[0] = bdy;
        air_attack.bdy_count = 1;
    }
    frames.push_back(air_attack);

    // Frame 11-12: Hurt
    FrameData hurt;
    hurt.pic = 11;
    hurt.state = 8;
    hurt.wait = 8;
    hurt.next = 999;
    hurt.hurtable = false; // Invincible during hurt
    {
        BdyData bdy;
        bdy.x = -15; bdy.y = -80; bdy.z = -10;
        bdy.width = 30; bdy.height = 80; bdy.depth = 20;
        hurt.bdys[0] = bdy;
        hurt.bdy_count = 1;
    }
    frames.push_back(hurt);

    // Frame 13: Defend
    FrameData defend;
    defend.pic = 13;
    defend.state = 11;
    defend.wait = 1;
    defend.next = 13;
    defend.hurtable = true;
    defend.hit_d = 13;
    {
        BdyData bdy;
        bdy.x = -18; bdy.y = -85; bdy.z = -12;
        bdy.width = 36; bdy.height = 85; bdy.depth = 24;
        defend.bdys[0] = bdy;
        defend.bdy_count = 1;
    }
    frames.push_back(defend);

    return frames;
}

} // namespace prank
