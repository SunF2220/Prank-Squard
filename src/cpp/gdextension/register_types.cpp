#include "register_types.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

// Core
#include "../core/fighter.h"
#include "../core/frame_interpreter.h"
#include "../core/input_buffer.h"
#include "../core/combat_system.h"
#include "../core/state_machine.h"

// AI
#include "../ai/ai_controller.h"

// Audio
#include "../audio/audio_manager.h"

namespace prank {

void initialize_gdextension() {
    godot::ClassDB::register_class<Fighter>();
    godot::ClassDB::register_class<FrameInterpreter>();
    godot::ClassDB::register_class<CombatSystem>();
    godot::ClassDB::register_class<AudioManager>();
}

void uninitialize_gdextension() {
}

} // namespace prank

using namespace prank;

extern "C" {

// GDExtension entry points
GDExtensionBool GDE_EXPORT prank_squad_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization* r_initialization) {

    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_gdextension);
    init_obj.register_terminator(uninitialize_gdextension);
    init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}

}
