#pragma once

#include "data_types.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

class Fighter;

class InputBuffer {
public:
    static constexpr int BUFFER_SIZE = 64;

    InputBuffer();
    ~InputBuffer() = default;

    void initialize();
    void record(const InputSnapshot& snap);
    int detect_special(const godot::Vector<PatternData>& patterns) const;
    bool match_pattern(const PatternData& pattern) const;

    bool is_dir(uint8_t dir_bit) const;
    bool attack_just_pressed() const;
    bool jump_just_pressed() const;
    bool defend_held() const;

    const InputSnapshot& latest() const { return buffer_[write_pos_ > 0 ? write_pos_ - 1 : BUFFER_SIZE - 1]; }

private:
    InputSnapshot buffer_[BUFFER_SIZE];
    int write_pos_ = 0;
};

} // namespace prank
