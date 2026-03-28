#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "Coordinates.h"
#include "PlayerHand.h"
#include "TrapSpell.h"
#include "DamagingSpell.h"
#include <vector>
#include <string>

#include "EnemyTower.h"
#include "GameData.h"
#include "SpellShop.h"
#include "Loader.h"

class Game {
public:
    enum class GameState { Going, Player_quit, Player_win, Player_lose, Level_up };
    
    Game(GameData& data, Loader& loader);
    ~Game() = default;

    // Геттеры для отрисовки
    std::string getFieldString() const;
    std::string getPlayerHud() const;
    const std::vector<std::string>& getMessageLog() const;
    
    // Управление состоянием
    bool isRunning() const;
    void setGameState(GameState newState);
    void updateWorld();

    // Методы команд
    void processMovement(int dx, int dy);
    void togglePlayerWeapon();
    void processShop();
    void processSpellCast();
    void processSave();

private:
    Player player;
    std::vector<Enemy> enemies;
    Field field;
    GameState state;
    SpellShop& spellShop;
    PlayerHand& playerHand;
    std::vector<TrapSpell> active_traps;
    std::vector<EnemyTower>& towers;

    Loader& loader;
    GameData& game_data;
    
    mutable std::vector<std::string> message_log;
    void log(const std::string& msg);
    void clearLog();

    template<typename T>
    int find_by_position(const std::vector<T>& container, const Coordinates& coordinates);
    
    char get_player_input(std::string info, std::string allowed_commands);
    Coordinates get_coordinates_from_player(std::string info);
    
    void attack(float damage, const Coordinates& target_pos);
    void shoot_arrow_from(Coordinates start, Coordinates dir);
    void enemies_move();
    void towers_move();
    bool cast_spell_internal();
};