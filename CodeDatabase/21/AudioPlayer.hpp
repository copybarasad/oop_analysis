#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <iostream>
#include <thread>

class AudioPlayer {
private:
    ma_engine engine;
    ma_sound sound;
    bool engine_initialized = false;
    bool sound_loaded = false;

public:
    AudioPlayer() {
        ma_result result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            return;
        }
        engine_initialized = true;
        if (loadAndPlay("../sounds/main_theme.wav", true)) {
            setVolume(0.5f);
        }
    }

    ~AudioPlayer() {
        stop();
        if (sound_loaded) {
            ma_sound_uninit(&sound);
        }
        if (engine_initialized) {
            ma_engine_uninit(&engine);
        }
    }

    bool loadAndPlay(const char* filepath, bool loop = false) {
        if (!engine_initialized) {
            return false;
        }
        if (sound_loaded) {
            ma_sound_uninit(&sound);
            sound_loaded = false;
        }
        ma_result result = ma_sound_init_from_file(&engine, filepath,
            MA_SOUND_FLAG_DECODE, NULL, NULL, &sound);

        if (result != MA_SUCCESS) {
            return false;
        }
        sound_loaded = true;
        if (loop) {
            ma_sound_set_looping(&sound, MA_TRUE);
        }
        result = ma_sound_start(&sound);
        if (result != MA_SUCCESS) {
            return false;
        }
        return true;
    }

    void stop() {
        if (sound_loaded) {
            ma_sound_stop(&sound);
        }
    }

    void setVolume(float volume) {
        if (sound_loaded) {
            ma_sound_set_volume(&sound, volume);
        }
    }
};