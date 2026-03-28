#ifndef GAMESAVEMANAGER_H
#define GAMESAVEMANAGER_H

#include "Game.h"
#include "Player.h"
#include "GameField.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "MeleeSpell.h"
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>

class Game;

class GameSaveManager {
public:
    class SaveException : public std::exception {
    private:
        std::string message_;
    public:
        explicit SaveException(const std::string& message) : message_(message) {}
        const char* what() const noexcept override { return message_.c_str(); }
    };

    GameSaveManager() = default;

    void saveGame(const Game& game, const std::string& filename);
    std::unique_ptr<Game> loadGame(const std::string& filename);
    bool saveExists(const std::string& filename) const;

private:
    // ¬спомогательные методы дл€ сериализации
    void savePlayer(std::ofstream& file, const Player& player) const;
    void saveGameField(std::ofstream& file, const GameField& field) const;
    void saveSpells(std::ofstream& file, const PlayerHand& hand) const;

    // ¬спомогательные методы дл€ десериализации
    std::unique_ptr<Player> loadPlayer(std::ifstream& file);
    std::unique_ptr<GameField> loadGameField(std::ifstream& file, Player* player);
    void loadSpells(std::ifstream& file, Player& player);
};

#endif