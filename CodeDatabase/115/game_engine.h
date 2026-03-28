#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "game_state.h"
#include "game_logic.h"
#include "game_save.h"
#include "command.h"
#include "game_event.h"
#include "event_manager.h" 
#include "i_observer.h"
#include <vector>
#include <iostream>

template <typename InputReaderT>
class GameEngine {
private:
    GameState& game_state;
    InputReaderT input_reader;
    std::vector<IObserver*> observers;
    bool load_requested;

    void notify(const GameEvent& event) {
        for (auto* obs : observers) {
            obs->onNotify(event);
        }
    }

    bool processCommand(const Command& cmd) {
        switch (cmd.type) {
            case CommandType::Move:
                game_state.getPlayer().move(cmd.dx, cmd.dy, game_state.getField());
                return true;

            case CommandType::Attack:
                GameLogic::processPlayerAction(game_state, 'f');
                return true;

            case CommandType::SwitchWeapon:
                GameLogic::processPlayerAction(game_state, 'r');
                return true;

            case CommandType::CastSpell:
                GameLogic::processPlayerAction(game_state, 'c');
                return true;

            case CommandType::OpenStore:
                GameLogic::processPlayerAction(game_state, 'b');
                return true;

            case CommandType::Save:
                try {
                    GameSave::save_game(game_state, 1, "savegame.json");
                    EventManager::trigger(EventType::Message, "Game Saved Successfully");
                } catch (...) {
                    EventManager::trigger(EventType::Message, "Save Failed");
                }
                return true;

            case CommandType::Load:
                load_requested = true;
                return false;

            case CommandType::Quit:
                return false;

            default:
                EventManager::trigger(EventType::Message, "Unknown command");
                return true;
        }
    }

public:
    GameEngine(GameState& state) : game_state(state), load_requested(false) {}

    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }

    InputReaderT& getInputReader() {
        return input_reader;
    }

    bool isLoadRequested() const {
        return load_requested;
    }

    bool update() {
        if (!game_state.getPlayer().get_alive()) {
            return false;
        }

        Command command = input_reader.readInput();

        if (command.type != CommandType::None) {
            EventManager::trigger(EventType::CommandExecuted, command.name);
        }

        bool keep_running = processCommand(command);

        if (keep_running) {
            GameLogic::processEnemyActions(game_state.getEnemies(), game_state.getPlayer(), game_state.getField());
            GameLogic::processTowerActions(game_state);
            GameLogic::processAllyActions(game_state);
            GameLogic::processTrapActions(game_state);

            GameEvent state_event;
            state_event.type = EventType::StateChanged;
            state_event.message = "Update";
            
            state_event.val1 = game_state.getPlayer().get_position_x();
            state_event.val2 = game_state.getPlayer().get_position_y();
            
            notify(state_event);
        }

        return keep_running && game_state.getPlayer().get_alive();
    }
};

#endif