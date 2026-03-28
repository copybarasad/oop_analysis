#pragma once

#include <SDL.h>

#include "render/hud/GameHUD.h"
#include "GameLogic.h"
#include "GameSave.h"
#include "GameState.h"
#include "world/World.h"
#include "entity/types/Player.h"
#include "core/sound/SoundManager.h"
#include "core/input/InputHandler.h"
#include "event/GameEventHandler.h"
#include "render/GameVisualizer.h"
#include "render/renderer/impl/SDLRenderer.h"
#include "input/impl/SDLGameInput.h"
#include "logger/GameLogger.h"
#include "render/particle/ParticleHelper.h"


class Game {
    friend class GameLogic;
    friend class GameSave;
    friend class GameHUD;

public:
    Game(const std::filesystem::path &save_file, const std::filesystem::path &controls_file);

    ~Game();

    bool init(const std::string &title);

    void run();

    void stop();


    Player &player() const {
        return *player_;
    }

    World &world() const {
        return *world_;
    }

    const SoundManager &sound() const {
        return sound_;
    }

    const GameHUD &hud() const {
        return hud_;
    }

    GameLogic &logic() {
        return logic_;
    }

    GameState &state() {
        return state_;
    }

    GameEventHandler &event_handler() {
        return event_handler_;
    }

    const InputHandler<SDLGameInput> &input() const {
        return input_;
    }


    bool player_won() const {
        return player_won_;
    }

    void on_window_resize() {
        hud_.reload_buttons();
    }

    void spawn_particle(const Particle &particle) {
        visualizer_.add_particle(particle);
    }

private:
    GameState state_ = GameState::PlayerMove;
    GameVisualizer<SDLRenderer> visualizer_;
    SoundManager sound_;
    GameLogic logic_;
    InputHandler<SDLGameInput> input_;
    GameSave save_;
    GameHUD hud_;
    GameEventHandler event_handler_;
    bool running_ = false;
    bool player_won_ = false;

    World *world_ = nullptr;
    Player *player_ = nullptr;

    Uint64 ticks_prev_ = 0;
    bool initialized_ = false;

    void load_sounds();


    void shutdown();

    void reset();
};
