#include "console_renderer.h"

void ConsoleRenderer::clear_screen() const
{
    std::ignore = system("clear");
}

ConsoleRenderer::CellVisual ConsoleRenderer::get_cell_visual(const Cell& cell) const
{
    char symbol = ' ';
    std::string color = COLOR_EMPTY;
    CellType type = cell.get_type();

    switch (type)
    {
        case CellType::PLAYER:
            symbol = 'P';
            color = COLOR_PLAYER;
            break;
        case CellType::ENEMY:
            symbol = 'E';
            color = COLOR_ENEMY;
            break;
        case CellType::WALL:
            symbol = '^';
            color = COLOR_TREE;
            break;
        case CellType::ENEMY_BUILDING:
            symbol = 'B';
            color = COLOR_BUILDING;
            break;
        case CellType::ENEMY_TOWER:
            symbol = 'T';
            color = COLOR_TOWER;
            break;
        default:
            if (cell.get_slow_trap())
            {
                symbol = '*';
                color = COLOR_TRAP;
            }
            else if (cell.get_trap_spell())
            {
                symbol = '*';
                color = COLOR_TRAP_SPELL;
            }
            else
            {
                symbol = '.';
                color = COLOR_EMPTY;
            }
            break;
    }
    return {symbol, color};
}

void ConsoleRenderer::render_map(const GameMap& map) const
{
    int width = map.get_width();
    int height = map.get_height();

    std::cout << COLOR_COORDINATES << "   ";
    for (int x = 0; x < width; ++x) 
    {
        if (x <= 10) std::cout << "  " << x;
        else std::cout << " " << x;
    }
    std::cout << COLOR_RESET << "\n";

    std::cout << COLOR_COORDINATES << "  " << COLOR_BORDER << "┌";
    for (int x = 0; x < width; ++x) std::cout << "───";
    std::cout << "──";
    std::cout << "┐" << COLOR_RESET << "\n";

    for (int y = 0; y < height; ++y)
    {
        std::cout << COLOR_COORDINATES << std::setw(2) << y << COLOR_BORDER << "│  " << COLOR_RESET;
        for (int x = 0; x < width; ++x)
        {
            CellVisual vis = get_cell_visual(map.get_cells()[y][x]);
            std::cout << vis.color << vis.symbol << COLOR_RESET << "  ";
        }
        std::cout << COLOR_BORDER << "│" << COLOR_RESET << "\n";
    }

    std::cout << COLOR_COORDINATES << "  " << COLOR_BORDER << "└";
    for (int x = 0; x < width; ++x) std::cout << "───";
    std::cout << "──";
    std::cout << "┘" << COLOR_RESET << "\n";
}

void ConsoleRenderer::render_legend() const
{
    std::cout << "\nLEGEND:\n";
    std::cout << COLOR_PLAYER << "P" << COLOR_RESET << " - Player   ";
    std::cout << COLOR_ENEMY << "E" << COLOR_RESET << " - Enemy   ";
    std::cout << COLOR_TREE << "^" << COLOR_RESET << " - Tree   ";
    std::cout << COLOR_BUILDING << "B" << COLOR_RESET << " - Building   ";
    std::cout << COLOR_TOWER << "T" << COLOR_RESET << " - Tower     ";
    std::cout << COLOR_TRAP << "*" << COLOR_RESET << " - Trap     ";
    std::cout << COLOR_TRAP_SPELL << "*" << COLOR_RESET << " - Spell Trap     ";
    std::cout << COLOR_EMPTY << "." << COLOR_RESET << " - Ground\n";
}

void ConsoleRenderer::render_player_stats(const Player* player) const
{
    if (!player) return;

    std::cout << "\nPLAYER:\n";
    std::cout << "Position: (" << player->get_x() << ", " << player->get_y() << ")   ";
    std::cout << "Health: " << player->get_health() << "/" << player->get_max_health() << "   ";
    std::cout << "Mana: " << player->get_mana() << "/" << player->get_max_mana() << "   ";
    std::cout << "Score: " << player->get_score() << "   ";
    std::cout << "Mode: " << (player->get_combat_mode() == CombatMode::MELEE ? "MELEE" : "RANGED") << "\n";
}

void ConsoleRenderer::render_spell_hand(const Player* player) const
{
    if (!player) return;

    const SpellHand& hand = player->get_spell_hand();
    std::cout << "\nSPELL HAND (" << hand.get_spell_count() << "/" << hand.get_max_size() << "):\n";
    
    if (hand.is_empty())
    {
        std::cout << "  No spells available\n";
    }
    else
    {
        for (size_t i = 0; i < hand.get_spell_count(); ++i)
        {
            const ISpell* spell = hand.get_spell(i);
            if (spell)
            {
                std::cout << "  " << (i + 1) << ". " << spell->get_name();
                std::cout << " [Cost: " << spell->get_mana_cost();
                std::cout << ", Range: " << spell->get_cast_range();
                std::cout << ", CD: " << spell->get_cooldown() << "/" << spell->get_max_cooldown() << "]";
                
                if (spell->is_available(player->get_mana()))
                    std::cout << " \033[1;32m(READY)\033[0m";
                else if (spell->get_cooldown() > 0)
                    std::cout << " \033[1;31m(COOLDOWN)\033[0m";
                else
                    std::cout << " \033[1;31m(NO MANA)\033[0m";
                
                std::cout << "\n";
            }
        }
    }
}

void ConsoleRenderer::render_enemy_stats(const GameMap& map) const
{
    std::cout << "\nENEMIES ALIVE: " << map.get_enemies().size() << "\n";

    if (auto* building = map.get_enemy_building())
    {
        if (building->is_alive())
        {
            std::cout << "\nENEMY BUILDING:\n";
            std::cout << "Position: (" << building->get_x() << ", " << building->get_y() << ")   ";
            std::cout << "Health: " << building->get_health() << "   ";
            std::cout << "Spawns in: " << building->get_spawn_cooldown() << "\n";
        }
    }

    if (auto* tower = map.get_enemy_tower())
    {
        std::cout << "\nENEMY TOWER:\n";
        std::cout << "Position: (" << tower->get_x() << ", " << tower->get_y() << ")   ";
        std::cout << "Range: " << tower->get_attack_range() << "   ";
        std::cout << "Cooldown: " << tower->get_cooldown() << "\n";
    }
}

void ConsoleRenderer::render(const Game& game) const
{
    clear_screen();

    std::cout << "LEVEL: " << game.get_current_level() << "\n\n"; 
    
    const GameMap& map = game.get_map();
    
    render_map(map);
    render_legend();
    render_player_stats(map.get_player());
    render_spell_hand(map.get_player());
    render_enemy_stats(map);
}