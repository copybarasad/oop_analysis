#include "Game.h"
#include "entity/types/Enemy.h"
#include <iostream>

#include "GameSave.h"
#include "spell/SpellRegistry.h"
#include "upgrade/PlayerUpgradeRegistry.h"
#include "render/GameVisualizer.h"

Game::Game(const std::filesystem::path &save_file, const std::filesystem::path &controls_file) : visualizer_(*this),
     logic_(*this),
    input_(*this, controls_file), save_(save_file), hud_(*this) {
}

Game::~Game() {
    shutdown();
}


bool Game::init(const std::string &title) {
    if (initialized_) {
        std::cerr << "Game already initialized\n";
        return false;
    }


    if (!input_.init()) {
        std::cerr << "Input Handler init failed\n";
        return false;
    }
    if (!save_.load(*this)) {
        delete player_;
        player_ = new Player(0, 0);
        logic_.init_level();
    }


    if (!visualizer_.init(title)) {
        std::cerr << "Visualizer init failed\n";
        return false;
    }

    load_sounds();


    ticks_prev_ = SDL_GetTicks64();
    initialized_ = true;
    return true;
}

void Game::run() {
    if (running_) {
        std::cerr << "Game is already running\n";
        return;
    }
    on_window_resize();
    running_ = true;
    while (running_) {
        input_.tick();


        const Uint64 now = SDL_GetTicks64();
        const float dt = static_cast<float>(now - ticks_prev_) / 1000.0f;
        ticks_prev_ = now;

        logic_.update(dt);

        visualizer_.render(dt);

        SDL_Delay(16);
    }
}


void Game::load_sounds() {
    sound_.load("hurt", "assets\\sounds\\hurt.wav");
    sound_.load("notify", "assets\\sounds\\notify.wav");
    sound_.load("action", "assets\\sounds\\character_action.wav");
    sound_.load("reward", "assets\\sounds\\reward.wav");
    sound_.load("death", "assets\\sounds\\death.wav");
}


void Game::stop() {
    running_ = false;
}

void Game::shutdown() {
    stop();
    if (state_ == GameState::Finished) {
        save_.delete_file();
    } else {
        save_.save(*this);
    }
    visualizer_.destroy();


    player_->get_spell_holder().clear();

    const auto entities = world_->entities();
    for (Entity *entity: entities) {
        if (entity == player_) player_ = nullptr;
        world_->remove_entity(entity);
        delete entity;
    }
    if (player_) {
        delete player_;
    }
    delete world_;
    SpellRegistry::instance().cleanup();
    PlayerUpgradeRegistry::instance().cleanup();
}

void Game::reset() {
    player_won_ = false;
    state_ = GameState::PlayerMove;
    logic_.set_wave(1);
    hud_.reload_buttons();
}

