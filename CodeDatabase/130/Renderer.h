#ifndef RENDERER_H
#define RENDERER_H

#include "Position.h"
#include "GameTypes.h"
#include "Command.h"
#include "SpellTypes.h"
#include <vector>
#include <string>
#include <unordered_map>

struct GameRenderData {
    int currentTurn = 0;
    int currentLevel = 0;
    
    int fieldWidth = 0;
    int fieldHeight = 0;
    std::vector<std::vector<CellType>> cells;
    
    Pos playerPos{0, 0};
    int playerHealth = 0;
    int playerMaxHealth = 0;
    int playerDamage = 0;
    int playerScore = 0;
    bool playerCanMove = true;
    bool playerSlowed = false;
    CombatMode playerMode = CombatMode::MELEE;
    
    int enemiesKilled = 0;
    int enemyCount = 0;
    int buildingCount = 0;
    int allyCount = 0;
    int towerCount = 0;
    int trapCount = 0;
    
    std::vector<Pos> enemyPositions;
    std::vector<Pos> allyPositions;
    std::vector<Pos> buildingPositions;
    std::vector<Pos> towerPositions;
    std::vector<Pos> trapPositions;
    
    struct SpellInfo {
        std::string name;
        std::string description;
        SpellType type;
    };
    std::vector<SpellInfo> handSpells;
    int handMaxSize = 5;
    
    int damageBonus = 0;
    int rangeBonus = 0;
    int areaBonus = 0;
    int summonBonus = 0;
    
    std::unordered_map<CommandType, char> keyBindings;
};

class Game;

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(Game& game) = 0;
};

class ConsoleRenderer : public IRenderer {
public:
    void render(Game& game) override;
    void renderFromData(const GameRenderData& data);
    void renderField(const GameRenderData& data);
    void renderPlayerInfo(const GameRenderData& data);
    void renderHand(const GameRenderData& data);
    void renderMenu(const GameRenderData& data);
    
private:
    std::string formatEnhancement(const GameRenderData& data) const;
    char getKeyForCommand(const GameRenderData& data, CommandType type, char fallback) const;
};

#endif
