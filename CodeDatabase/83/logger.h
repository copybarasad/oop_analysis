#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "event_dispatcher.h"

class Logger {
public:
    enum class Mode {
        CONSOLE,
        FILE
    };

    Logger(EventDispatcher& dispatcher,
           Mode mode,
           const std::string& filename = "game.log")
        : mode(mode)
    {
        if (mode == Mode::FILE) {
            file.open(filename);
        }

        dispatcher.subscribe(
            [this](const Event& e) { onEvent(e); }
        );
    }

    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }

private:
    Mode mode;
    std::ofstream file;

private:
    void log(const std::string& text) {
        if (mode == Mode::CONSOLE) {
            std::cout << text << std::endl;
        }
        else if (mode == Mode::FILE && file.is_open()) {
            file << text << std::endl;
        }
    }

    void onEvent(const Event& e) {
        switch (e.type) {

        case EventType::PLAYER_MOVE:
            log("[PLAYER] moved to (" + pos(e) + ")");
            break;

        case EventType::PLAYER_DAMAGE:
            log("[PLAYER] took " + std::to_string(e.value) + " dmg");
            break;

        case EventType::PLAYER_DEATH:
            log("[PLAYER] died");
            break;

        case EventType::ENEMY_MOVE:
            log("[ENEMY] moved to (" + pos(e) + ")");
            break;

        case EventType::ENEMY_ATTACK:
            log("[ENEMY] attacked player for " +
                std::to_string(e.value));
            break;

        case EventType::ENEMY_DAMAGE:
            log("[ENEMY] took " + std::to_string(e.value) +
                " dmg from " + e.source);
            break;

        case EventType::ENEMY_DEATH:
            log("[ENEMY] died at (" + pos(e) + ")");
            break;

        case EventType::SPELL_CAST:
            log("[SPELL] " + e.source + " cast " + e.target);
            break;

        case EventType::SPELL_MISS:
            log("[SPELL] missed");
            break;

        case EventType::TRAP_TRIGGER:
            log("[TRAP] triggered at (" + pos(e) + ")");
            break;

        case EventType::TOWER_DESTROYED:
            log("[TOWER] destroyed at (" + pos(e) + ")");
            break;

        default:
            break;
        }
    }

    std::string pos(const Event& e) const {
        return std::to_string(e.x) + "," + std::to_string(e.y);
    }
};
