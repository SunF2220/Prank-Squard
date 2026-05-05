#include "input_buffer.h"

namespace prank {

InputBuffer::InputBuffer() {
    initialize();
}

void InputBuffer::initialize() {
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        buffer_[i] = InputSnapshot{};
    }
    write_pos_ = 0;
}

void InputBuffer::record(const InputSnapshot& snap) {
    buffer_[write_pos_] = snap;
    write_pos_ = (write_pos_ + 1) % BUFFER_SIZE;
}

int InputBuffer::detect_special(const godot::Vector<PatternData>& patterns) const {
    for (int p = 0; p < patterns.size(); ++p) {
        if (match_pattern(patterns[p])) {
            return patterns[p].target_frame;
        }
    }
    return -1;
}

bool InputBuffer::match_pattern(const PatternData& pattern) const {
    if (pattern.length == 0) return false;

    int pos = (write_pos_ > 0 ? write_pos_ - 1 : BUFFER_SIZE - 1);
    for (int i = static_cast<int>(pattern.length) - 1; i >= 0; --i) {
        uint8_t expected = pattern.sequence[i];
        uint8_t actual = buffer_[pos].directions;

        if (i == static_cast<int>(pattern.length) - 1 && pattern.ends_with_button) {
            if (!(buffer_[pos].just_pressed & pattern.required_button))
                return false;
            actual &= ~InputSnapshot::ATTACK & ~InputSnapshot::JUMP & ~InputSnapshot::DEFEND;
        }

        if ((actual & expected) != expected) return false;

        pos = (pos > 0 ? pos - 1 : BUFFER_SIZE - 1);
    }
    return true;
}

bool InputBuffer::is_dir(uint8_t dir_bit) const {
    return (latest().directions & dir_bit) != 0;
}

bool InputBuffer::attack_just_pressed() const {
    return (latest().just_pressed & InputSnapshot::ATTACK) != 0;
}

bool InputBuffer::jump_just_pressed() const {
    return (latest().just_pressed & InputSnapshot::JUMP) != 0;
}

bool InputBuffer::defend_held() const {
    return (latest().buttons & InputSnapshot::DEFEND) != 0;
}

} // namespace prank
