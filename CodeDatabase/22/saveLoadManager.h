#include <chrono>
#include <filesystem>
#include "enemyManager.h"
#include "spellManager.h"
#include <fstream>
#include <format>


class SaveLoadManager {

    public:
        SaveLoadManager();
        void save(std::string filename, SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int level_);
        void load(std::string filePath, SpellManager &spellManager_, EnemyManager &enemyManager_, Field &field_, Player &player_, int &level_);
};