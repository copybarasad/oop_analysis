#include "StoreGame.h"
#include "directdamage.h"
#include "areadamage.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

bool StoreGame::SaveGame(const GameState& state, const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл для сохранения: " << filename << std::endl;
            return false;
        }

        file << state.GetFieldWidth() << " " << state.GetFieldHeight() << "\n";
        file << state.GetBlockChance() << "\n";
        file << state.GetBaseEnemyCount() << "\n";

        file << state.GetCurrentLevel() << "\n";
        file << state.GetPlayerX() << " " << state.GetPlayerY() << "\n";
        
        const Player& player = state.GetPlayer();
        file << player.GetHealth() << " " << player.GetMurder() << "\n";

        const auto& enemies = state.GetEnemies();
        const auto& positions = state.GetEnemyPositions();
        file << enemies.size() << "\n";
        
        for (size_t i = 0; i < enemies.size(); ++i) {
            file << positions[i].first << " " << positions[i].second << " "
                 << enemies[i].GetHealth() << "\n";
        }

        const auto& spells = state.GetHand().GetSpells();
        file << spells.size() << "\n";
        for (const auto& sp : spells) {
            file << sp->GetName() << "\n";
        }

        file.close();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при сохранении: " << e.what() << std::endl;
        return false;
    }
}

bool StoreGame::LoadGame(GameState& state, const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Файл сохранения не найден: " << filename << std::endl;
            return false;
        }

        int width, height, baseEnemyCount;
        double blockChance;
        
        file >> width >> height >> blockChance >> baseEnemyCount;
        state.SetFieldSize(width, height);
        state.SetBlockChance(blockChance);
        state.SetBaseEnemyCount(baseEnemyCount);
        
        int level, px, py, health, murders;
        size_t enemyCount;

        file >> level >> px >> py >> health >> murders >> enemyCount;

        if (!file.good()) {
            std::cerr << "Ошибка чтения данных из файла" << std::endl;
            return false;
        }

        std::vector<Enemy> enemies;
        std::vector<std::pair<int, int>> positions;
        
        for (size_t i = 0; i < enemyCount; ++i) {
            int ex, ey, eh;
            file >> ex >> ey >> eh;
            
            Enemy e;
            int diff = 100 - eh;
            if (diff > 0) e.Damage(diff);
            
            enemies.push_back(e);
            positions.emplace_back(ex, ey);
        }

        size_t spellCount;
        file >> spellCount;
        
        Hand newHand(3);
        newHand.ClearSpells();
        
        for (size_t i = 0; i < spellCount; i++) {
            std::string name;
            std::getline(file >> std::ws, name);
            
            auto sp = CreateSpellByName(name);
            if (sp) {
                newHand.AddLoadedSpell(std::move(sp));
            }
        }

        Player player;
        int diff = 100 - health;
        if (diff > 0) player.Damage(diff);
        for (int i = 0; i < murders; ++i) player.NewMurder();

        state.SetCurrentLevel(level);
        state.SetPlayer(player);
        state.SetPlayerPosition(px, py);
        state.SetEnemies(enemies);
        state.SetEnemyPositions(positions);
        state.SetHand(newHand);

        file.close();
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<ISpell> StoreGame::CreateSpellByName(const std::string& name) {
    if (name == "Заклинание прямого урона")
        return std::make_unique<DirectDamage>(10, 5);

    if (name == "Заклинание урона по площади")
        return std::make_unique<AreaDamage>(15, 5);

    return nullptr;
}