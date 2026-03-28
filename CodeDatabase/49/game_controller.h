#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

#include "../commands/command_interface.h"
#include "../commands/move_command.h"
#include "../commands/attack_command.h"
#include "../commands/switch_mode_command.h"
#include "../commands/cast_spell_command.h"
#include "../commands/save_command.h"
#include "../commands/load_command.h"
#include "../commands/exit_command.h"
#include "../commands/invalid_command.h"
#include "game.h"

template <typename InputReader>
class GameController
{
private:
    Game& game;
    InputReader reader;

    static constexpr int INVALID_COMMAND_DELAY_MS = 500;

    void show_invalid_message(const std::string& message) const;
    void update_reader_spell_count();

    bool handle_move(const MoveCommand& cmd);
    bool handle_attack();
    bool handle_switch_mode();
    bool handle_cast_spell(const CastSpellCommand& cmd);
    bool handle_save();
    bool handle_load();
    bool handle_exit();

public:
    explicit GameController(Game& game_ref);

    template <typename Visualizer>
    bool process_player_turn(Visualizer& visualizer);
    
    bool execute_command(const std::unique_ptr<ICommand>& cmd);
};

template <typename InputReader>
GameController<InputReader>::GameController(Game& game_ref)
    : game(game_ref), reader()
{
    update_reader_spell_count();
}

template <typename InputReader>
void GameController<InputReader>::update_reader_spell_count()
{
    int spell_count = static_cast<int>(
        game.get_map().get_player()->get_spell_hand().get_spell_count());
    reader.set_max_spell_count(spell_count);
}

template <typename InputReader>
void GameController<InputReader>::show_invalid_message(const std::string& message) const
{
    std::cout << "\n\033[1;31m" << message << "\033[0m\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(INVALID_COMMAND_DELAY_MS));
}

template <typename InputReader>
template <typename Visualizer>
bool GameController<InputReader>::process_player_turn(Visualizer& visualizer)
{
    update_reader_spell_count();

    bool turn_completed = false;
    while (!turn_completed)
    {
        visualizer.render();

        std::cout << "\n" << reader.get_controls_help() 
                  << ",\n1-" << game.get_map().get_player()->get_spell_hand().get_spell_count()
                  << " to cast spell: ";

        auto cmd = reader.read_command();
        
        turn_completed = execute_command(cmd);
    }
    return true;
}

template <typename InputReader>
bool GameController<InputReader>::execute_command(const std::unique_ptr<ICommand>& cmd)
{
    switch (cmd->get_type())
    {
        case CommandType::MOVE:
            return handle_move(static_cast<const MoveCommand&>(*cmd));
        
        case CommandType::ATTACK:
            return handle_attack();
        
        case CommandType::SWITCH_MODE:
            return handle_switch_mode();
        
        case CommandType::CAST_SPELL:
            return handle_cast_spell(static_cast<const CastSpellCommand&>(*cmd));
        
        case CommandType::SAVE:
            return handle_save();
        
        case CommandType::LOAD:
            return handle_load();
        
        case CommandType::EXIT:
            return handle_exit();
        
        case CommandType::INVALID:
        {
            const auto& inv = static_cast<const InvalidCommand&>(*cmd);
            show_invalid_message(inv.get_reason());
            return false;
        }
        
        default:
            show_invalid_message("Unknown command");
            return false;
    }
}

template <typename InputReader>
bool GameController<InputReader>::handle_move(const MoveCommand& cmd)
{
    Player* player = game.get_map().get_player();
    int new_x = player->get_x() + cmd.get_dx();
    int new_y = player->get_y() + cmd.get_dy();

    if (player->move(game.get_map_mutable(), new_x, new_y))
    {
        return true;
    }

    show_invalid_message("Blocked cell");
    return false;
}

template <typename InputReader>
bool GameController<InputReader>::handle_attack()
{
    game.get_map().get_player()->attack(game.get_map_mutable());
    return true;
}

template <typename InputReader>
bool GameController<InputReader>::handle_switch_mode()
{
    game.get_map().get_player()->switch_combat_mode();
    return true;
}

template <typename InputReader>
bool GameController<InputReader>::handle_cast_spell(const CastSpellCommand& cmd)
{
    Player* player = game.get_map().get_player();
    SpellHand& hand = player->get_spell_hand();
    
    int index = cmd.get_spell_index();
    int x = cmd.get_target_x();
    int y = cmd.get_target_y();

    if (index < 0 || index >= static_cast<int>(hand.get_spell_count()))
    {
        show_invalid_message("Invalid spell index");
        return false;
    }

    ISpell* spell = hand.get_spell(index);

    if (!spell->is_available(player->get_mana()))
    {
        show_invalid_message("Not enough mana or spell on cooldown");
        return false;
    }

    if (!game.get_map().is_valid_position(x, y))
    {
        show_invalid_message("Invalid coordinates");
        return false;
    }

    int dist = std::abs(x - player->get_x()) + std::abs(y - player->get_y());
    if (dist > spell->get_cast_range())
    {
        show_invalid_message("Coordinates out of range");
        return false;
    }

    auto cell_type = game.get_map().get_cell(x, y).get_type();
    std::string spell_name = spell->get_name();

    bool can_cast = false;
    if (spell_name == "Area Damage")
    {
        can_cast = true;
    }
    else if (spell_name == "Direct Damage")
    {
        can_cast = (cell_type == CellType::ENEMY || cell_type == CellType::ENEMY_BUILDING);
    }
    else if (spell_name == "Trap")
    {
        can_cast = (cell_type == CellType::EMPTY && !game.get_map().get_cell(x, y).get_slow_trap());
    }

    if (can_cast)
    {
        player->cast_spell(spell, game.get_map_mutable(), x, y);
        return true;
    }

    show_invalid_message("Can't cast spell here");
    return false;
}

template <typename InputReader>
bool GameController<InputReader>::handle_save()
{
    game.save_to_file();
    return false;
}

template <typename InputReader>
bool GameController<InputReader>::handle_load()
{
    game.load_from_file();
    return false;
}

template <typename InputReader>
bool GameController<InputReader>::handle_exit()
{
    if (game.confirm_exit())
    {
        game.stop();
        std::exit(0);
    }
    return false;
}

#endif