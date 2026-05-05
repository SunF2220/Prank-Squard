#pragma once

#include "data_types.h"
#include "fighter.h"
#include "input_buffer.h"
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/templates/vector.hpp>

namespace prank {

class FrameInterpreter : public godot::Node {
    GDCLASS(FrameInterpreter, godot::Node);

public:
    FrameInterpreter();
    ~FrameInterpreter() = default;

    void set_frame_table(const godot::Vector<FrameData*>& frames);
    void set_patterns(const godot::Vector<PatternData>& patterns);
    void initialize(Fighter* fighter);

    void update(Fighter* fighter, float delta);

private:
    int current_frame_index_ = 0;
    int frame_timer_ = 0;
    godot::Vector<FrameData*> frame_table_;
    godot::Vector<PatternData> patterns_;

    int check_input_transitions(Fighter* fighter);
    void transition_to(Fighter* fighter, int new_index);
    void process_frame(Fighter* fighter);

protected:
    static void _bind_methods();
};

} // namespace prank
