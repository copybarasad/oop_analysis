#pragma once
#include "game.h"
#include "game_exceptions.h"
#include <string>
#include <fstream>
#include <vector>

class GameSaver {
private:
    std::string filename_;
    const std::string FILE_SIGNATURE = "VHSAVE";
    const int FILE_VERSION = 1;

public:
    explicit GameSaver(const std::string& filename);
    
    void saveGame(const Game& game);
    void loadGame(Game& game);
    bool saveExists() const;

private:
    // Методы сохранения
    void saveGameState(std::ofstream& file, const Game& game);
    void saveField(std::ofstream& file, const Field& field);
    void saveHunter(std::ofstream& file, const VampireHunter& hunter);
    void saveDracula(std::ofstream& file, const Dracula& dracula);
    
    // Методы загрузки
    void loadGameState(std::ifstream& file, Game& game);
    void loadField(std::ifstream& file, Field& field);
    void loadHunter(std::ifstream& file, VampireHunter& hunter);
    void loadDracula(std::ifstream& file, Dracula& dracula);
    
    // Вспомогательные методы
    void validateSaveFile(std::ifstream& file);
    void savePosition(std::ofstream& file, const Position& pos);
    Position loadPosition(std::ifstream& file);
    
    // Методы загрузки отдельных объектов
    VampireTower loadTower(std::ifstream& file);
    Library loadLibrary(std::ifstream& file);
    Spell* loadSpell(std::ifstream& file);
};