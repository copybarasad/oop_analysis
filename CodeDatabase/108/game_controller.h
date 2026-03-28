#ifndef OOP_GAME_CONTROLLER_H
#define OOP_GAME_CONTROLLER_H
#include <memory>

#include "commands/wasd_cmd.h"
#include "commands/e_cmd.h"
#include "commands/f_cmd.h"
#include "commands/load_cmd.h"
#include "commands/save_cmd.h"
#include "commands/cast_spell_cmd.h"
#include "commands/quit_cmd.h"


class Command;
class GameLoop;


template<typename InputProcessorType>
class GameController {
    std::unique_ptr<InputProcessorType> input_processor;
    GameLoop* game;


    void execute_console_command(Command& command) const {
        if (auto wasd_cmd = dynamic_cast<WasdCommand*>(&command)) {
            game->execute_wasd(wasd_cmd->get_symbol());
        }
        else if (dynamic_cast<NewCombatModeCommand*>(&command)) {
            game->execute_switch_combat_mode();
        }
        else if (dynamic_cast<KickCommand*>(&command)) {
            game->execute_kick();
        }
        else if (dynamic_cast<LoadCommand*>(&command)) {
            game->execute_load();
        }
        else if (dynamic_cast<SaveCommand*>(&command)) {
            game->execute_save();
        }
        else if (auto cast_spell_cmd = dynamic_cast<CastSpellCommand*>(&command)) {
            game->execute_cast_spell(cast_spell_cmd->get_spell_index());
        }
        else if (dynamic_cast<QuitCommand*>(&command)) {
            game->execute_quit();
        }
    }

public:
    explicit GameController(GameLoop* the_game) : input_processor(std::make_unique<InputProcessorType>()), game(the_game) {}

    void process_command() {
        std::unique_ptr<Command> command = input_processor->process_input();
        if (command) {
            execute_console_command(*command);
        }
    }


};



#endif //OOP_GAME_CONTROLLER_H