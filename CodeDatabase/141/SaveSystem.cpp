#include "SaveSystem.h"
#include "Game.h"
#include "GameSnapshot.h"
#include "SaveException.h"
#include "LoadException.h"
#include "FileNotFoundException.h"
#include "InvalidSaveFileException.h"
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cctype>

const std::string SAVE_FILE_HEADER = "GAME_SAVE_V1";
const std::string SAVE_DIRECTORY = "saves";
const std::string SAVE_EXTENSION = ".sav";

namespace {
    std::filesystem::path getSaveDirectoryPath() {
        return std::filesystem::current_path() / SAVE_DIRECTORY;
    }

    std::string sanitizeSlotName(const std::string &slotName) {
        std::string sanitized;
        sanitized.reserve(slotName.size());
        for (char ch: slotName) {
            unsigned char uch = static_cast<unsigned char>(ch);
            if (std::isalnum(uch) || ch == '_' || ch == '-') {
                sanitized.push_back(ch);
            }
        }
        return sanitized;
    }
}

void SaveSystem::saveGame(const Game &game, const std::string &filename) {
    std::filesystem::path path(filename);
    if (!path.parent_path().empty()) {
        std::error_code ec;
        std::filesystem::create_directories(path.parent_path(), ec);
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw SaveException("Cannot open file for writing: " + filename);
    }

    try {
        writeFileHeader(file);
        GameSnapshot snapshot = game.makeSnapshot();
        writeGameState(file, snapshot);
    } catch (const std::exception &e) {
        file.close();
        throw SaveException("Error writing save file: " + std::string(e.what()));
    }

    if (!file.good()) {
        file.close();
        throw SaveException("Error writing to file: " + filename);
    }
}

void SaveSystem::loadGame(Game &game, const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }

    try {
        validateFileHeader(file);
        GameSnapshot snapshot = readGameState(file);
        game.applySnapshot(snapshot);
    } catch (const LoadException &e) {
        file.close();
        throw;
    } catch (const std::exception &e) {
        file.close();
        throw InvalidSaveFileException("Unexpected error: " + std::string(e.what()));
    }

    if (!file.good() && !file.eof()) {
        file.close();
        throw InvalidSaveFileException("Error reading from file");
    }
}

bool SaveSystem::saveFileExists(const std::string &filename) {
    std::filesystem::path path(filename);
    return std::filesystem::exists(path);
}

void SaveSystem::writeFileHeader(std::ofstream &file) {
    file.write(SAVE_FILE_HEADER.c_str(), SAVE_FILE_HEADER.length());
    file.put('\n');
}

void SaveSystem::validateFileHeader(std::ifstream &file) {
    std::string header;
    std::getline(file, header);
    if (header != SAVE_FILE_HEADER) {
        throw InvalidSaveFileException("Invalid file format or corrupted save file");
    }
}

void SaveSystem::writeGameState(std::ofstream &file, const GameSnapshot &snapshot) {
    file << snapshot.currentLevel << "\n";
    file << snapshot.fieldWidth << " " << snapshot.fieldHeight << "\n";
    file << snapshot.enemiesKilled << "\n";

    const auto &p = snapshot.player;
    file << p.x << " " << p.y << " " << p.health << " "
            << p.meleeDamage << " " << p.rangedDamage << " "
            << p.score << " " << static_cast<int>(p.battleType) << " "
            << (p.skipTurn ? 1 : 0) << " " << p.handCapacity << "\n";

    file << p.hand.size() << "\n";
    for (const auto &spell: p.hand) {
        file << static_cast<int>(spell.type) << " " << spell.parameters.size();
        for (int param: spell.parameters) {
            file << " " << param;
        }
        file << "\n";
    }

    file << snapshot.fieldWidth << " " << snapshot.fieldHeight << "\n";
    for (int y = 0; y < snapshot.fieldHeight; ++y) {
        for (int x = 0; x < snapshot.fieldWidth; ++x) {
            file << static_cast<int>(snapshot.cells[y][x]) << " ";
        }
        file << "\n";
    }

    file << snapshot.enemies.size() << "\n";
    for (const auto &enemy: snapshot.enemies) {
        file << enemy.x << " " << enemy.y << " " << enemy.health << " " << enemy.damage << "\n";
    }

    file << snapshot.buildings.size() << "\n";
    for (const auto &building: snapshot.buildings) {
        file << building.x << " " << building.y << " " << building.spawnCooldown << " "
                << building.currentCooldown << "\n";
    }

    file << snapshot.towers.size() << "\n";
    for (const auto &tower: snapshot.towers) {
        file << tower.x << " " << tower.y << " " << tower.radius << " "
                << tower.damage << " " << tower.cooldown << " " << tower.currentCooldown << "\n";
    }

    file << snapshot.traps.size() << "\n";
    for (const auto &trap: snapshot.traps) {
        file << trap.x << " " << trap.y << " " << trap.damage << " " << (trap.active ? 1 : 0) << "\n";
    }

    file << snapshot.allies.size() << "\n";
    for (const auto &ally: snapshot.allies) {
        file << ally.x << " " << ally.y << " " << ally.health << " " << ally.damage << "\n";
    }

    file << snapshot.pendingEnhancements.size() << "\n";
    for (const auto &enhancement: snapshot.pendingEnhancements) {
        file << static_cast<int>(enhancement.type) << " " << enhancement.value << "\n";
    }
}

GameSnapshot SaveSystem::readGameState(std::ifstream &file) {
    GameSnapshot snapshot{};

    int level;
    file >> level;
    if (file.fail()) throw InvalidSaveFileException("Cannot read current level");
    snapshot.currentLevel = level;

    file >> snapshot.fieldWidth >> snapshot.fieldHeight;
    if (file.fail() || snapshot.fieldWidth <= 0 || snapshot.fieldHeight <= 0) {
        throw InvalidSaveFileException("Invalid field dimensions");
    }

    file >> snapshot.enemiesKilled;
    if (file.fail()) throw InvalidSaveFileException("Cannot read enemies killed");

    int battleType, skipTurn;
    file >> snapshot.player.x >> snapshot.player.y >> snapshot.player.health
            >> snapshot.player.meleeDamage >> snapshot.player.rangedDamage
            >> snapshot.player.score >> battleType >> skipTurn >> snapshot.player.handCapacity;
    if (file.fail()) throw InvalidSaveFileException("Cannot read player data");
    snapshot.player.battleType = static_cast<BattleType>(battleType);
    snapshot.player.skipTurn = skipTurn != 0;

    int handSize;
    file >> handSize;
    if (file.fail() || handSize < 0 || handSize > 20) {
        throw InvalidSaveFileException("Invalid hand size");
    }
    for (int i = 0; i < handSize; ++i) {
        int type, paramCount;
        file >> type >> paramCount;
        if (file.fail() || paramCount < 0 || paramCount > 10) {
            throw InvalidSaveFileException("Invalid spell data");
        }
        SpellState state{};
        state.type = static_cast<SpellType>(type);
        if (type < 0 || type > static_cast<int>(SpellType::ENHANCE)) {
            throw InvalidSaveFileException("Unknown spell type");
        }
        for (int j = 0; j < paramCount; ++j) {
            int param;
            file >> param;
            if (file.fail()) throw InvalidSaveFileException("Cannot read spell parameter");
            state.parameters.push_back(param);
        }
        snapshot.player.hand.push_back(state);
    }

    int width, height;
    file >> width >> height;
    if (file.fail() || width != snapshot.fieldWidth || height != snapshot.fieldHeight) {
        throw InvalidSaveFileException("Field size mismatch (save file is corrupted or from an old version)");
    }

    snapshot.cells.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int cellType;
            file >> cellType;
            if (file.fail()) throw InvalidSaveFileException("Cannot read cell type");
            snapshot.cells[y][x] = static_cast<CellType>(cellType);
        }
    }

    int enemyCount;
    file >> enemyCount;
    if (file.fail() || enemyCount < 0 || enemyCount > 1000) {
        throw InvalidSaveFileException("Invalid enemy count");
    }
    for (int i = 0; i < enemyCount; ++i) {
        EnemyState st{};
        file >> st.x >> st.y >> st.health >> st.damage;
        if (file.fail()) throw InvalidSaveFileException("Cannot read enemy data");
        snapshot.enemies.push_back(st);
    }

    int buildingCount;
    file >> buildingCount;
    if (file.fail() || buildingCount < 0 || buildingCount > 100) {
        throw InvalidSaveFileException("Invalid building count");
    }
    for (int i = 0; i < buildingCount; ++i) {
        BuildingState st{};
        file >> st.x >> st.y >> st.spawnCooldown >> st.currentCooldown;
        if (file.fail()) throw InvalidSaveFileException("Cannot read building data");
        snapshot.buildings.push_back(st);
    }

    int towerCount;
    file >> towerCount;
    if (file.fail() || towerCount < 0 || towerCount > 100) {
        throw InvalidSaveFileException("Invalid tower count");
    }
    for (int i = 0; i < towerCount; ++i) {
        TowerState st{};
        file >> st.x >> st.y >> st.radius >> st.damage >> st.cooldown >> st.currentCooldown;
        if (file.fail()) throw InvalidSaveFileException("Cannot read tower data");
        snapshot.towers.push_back(st);
    }

    int trapCount;
    file >> trapCount;
    if (file.fail() || trapCount < 0 || trapCount > 1000) {
        throw InvalidSaveFileException("Invalid trap count");
    }
    for (int i = 0; i < trapCount; ++i) {
        TrapState st{};
        int active;
        file >> st.x >> st.y >> st.damage >> active;
        if (file.fail()) throw InvalidSaveFileException("Cannot read trap data");
        st.active = active != 0;
        snapshot.traps.push_back(st);
    }

    int allyCount;
    file >> allyCount;
    if (file.fail() || allyCount < 0 || allyCount > 1000) {
        throw InvalidSaveFileException("Invalid ally count");
    }
    for (int i = 0; i < allyCount; ++i) {
        AllyState st{};
        file >> st.x >> st.y >> st.health >> st.damage;
        if (file.fail()) throw InvalidSaveFileException("Cannot read ally data");
        snapshot.allies.push_back(st);
    }

    int enhancementCount;
    file >> enhancementCount;
    if (file.fail() || enhancementCount < 0 || enhancementCount > 100) {
        throw InvalidSaveFileException("Invalid enhancement count");
    }
    for (int i = 0; i < enhancementCount; ++i) {
        int type, value;
        file >> type >> value;
        if (file.fail()) throw InvalidSaveFileException("Cannot read enhancement data");
        Enhancement enhancement;
        enhancement.type = static_cast<EnhancementType>(type);
        enhancement.value = value;
        snapshot.pendingEnhancements.push_back(enhancement);
    }

    return snapshot;
}

std::vector<std::string> SaveSystem::listSaveSlots() {
    std::vector<std::string> slots;
    std::filesystem::path dir = getSaveDirectoryPath();
    if (!std::filesystem::exists(dir)) {
        return slots;
    }

    for (const auto &entry: std::filesystem::directory_iterator(dir)) {
        if (!entry.is_regular_file()) continue;
        const auto &path = entry.path();
        if (path.extension() == SAVE_EXTENSION) {
            slots.push_back(path.stem().string());
        }
    }

    std::sort(slots.begin(), slots.end());
    return slots;
}

std::string SaveSystem::getSaveFilePath(const std::string &slotName) {
    std::string sanitized = sanitizeSlotName(slotName);
    if (sanitized.empty()) {
        throw SaveException("Invalid save slot name");
    }
    std::filesystem::path dir = getSaveDirectoryPath();
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    std::filesystem::path fullPath = dir / (sanitized + SAVE_EXTENSION);
    return fullPath.string();
}
