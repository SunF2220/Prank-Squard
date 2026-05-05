#include "audio_manager.h"

namespace prank {

void AudioManager::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("play_sfx", "sound_id", "volume_db"), &AudioManager::play_sfx);
    godot::ClassDB::bind_method(godot::D_METHOD("play_bgm", "path"), &AudioManager::play_bgm);
    godot::ClassDB::bind_method(godot::D_METHOD("stop_bgm"), &AudioManager::stop_bgm);
    godot::ClassDB::bind_method(godot::D_METHOD("set_master_volume", "linear"), &AudioManager::set_master_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("set_sfx_volume", "linear"), &AudioManager::set_sfx_volume);
    godot::ClassDB::bind_method(godot::D_METHOD("set_bgm_volume", "linear"), &AudioManager::set_bgm_volume);
}

AudioManager::AudioManager() {
}

void AudioManager::play_sfx(int32_t sound_id, float volume_db) {
    // Placeholder - sound system to be implemented
}

void AudioManager::play_bgm(const godot::String& path) {
    // Placeholder
}

void AudioManager::stop_bgm() {
    // Placeholder
}

void AudioManager::set_master_volume(float linear) {
    master_vol_ = linear;
}

void AudioManager::set_sfx_volume(float linear) {
    sfx_vol_ = linear;
}

void AudioManager::set_bgm_volume(float linear) {
    bgm_vol_ = linear;
}

} // namespace prank
