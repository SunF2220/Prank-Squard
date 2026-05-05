#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <godot_cpp/variant/string.hpp>

namespace prank {

class AudioManager : public godot::Node {
    GDCLASS(AudioManager, godot::Node);

public:
    AudioManager();
    ~AudioManager() = default;

    void play_sfx(int32_t sound_id, float volume_db = 0.0f);
    void play_bgm(const godot::String& path);
    void stop_bgm();
    void set_master_volume(float linear);
    void set_sfx_volume(float linear);
    void set_bgm_volume(float linear);

protected:
    static void _bind_methods();

private:
    godot::AudioStreamPlayer* sfx_player_ = nullptr;
    godot::AudioStreamPlayer* bgm_player_ = nullptr;
    float master_vol_ = 1.0f;
    float sfx_vol_ = 1.0f;
    float bgm_vol_ = 1.0f;
};

} // namespace prank
