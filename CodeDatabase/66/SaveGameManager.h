#ifndef SAVEGAMEMANAGER_H
#define SAVEGAMEMANAGER_H

#include "GameMode.h"
#include <string>
#include <memory>

class Game;
class Player;
class GameWorld;
class SpellHand;
class EnhancementBuffer;

struct GameState {
    int currentLevel;
    int playerHealth;
    int playerScore;
    int playerMeleeDamage;
    int playerRangedDamage;
    int combatMode;
    int fieldSize;
    int turnNumber;
    
    GameState();
};

class SaveGameManager {
private:
    static constexpr const char* SAVE_FILE = "savegame.dat";
    static constexpr int SAVE_VERSION = 1;
    
    void writeHeader(std::fstream& file);
    void writeGameState(std::fstream& file, const GameState& state);
    void writeEnhancements(std::fstream& file, const EnhancementBuffer& buffer);
    void writeSpellHand(std::fstream& file, const SpellHand& spellHand);
    void writeWorldState(std::fstream& file, const GameWorld& world, int fieldSize);
    
    int readHeader(std::fstream& file);
    GameState readGameState(std::fstream& file);
    void readEnhancements(std::fstream& file, EnhancementBuffer& buffer);
    void readSpellHand(std::fstream& file, SpellHand& spellHand);
    void readWorldState(std::fstream& file, GameWorld& world, int fieldSize);
    
public:
    SaveGameManager();
    
    void saveGame(const Game& game, int currentLevel);
    bool loadGame(Game& game, int& outLevel);
    bool hasSaveFile() const;
    void deleteSaveFile();
    GameMode readGameMode() const;
    int readFieldSize() const;
};

#endif
