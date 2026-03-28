#include "Tower.h"
#include "Map.h"   
#include "Enemy.h" 
#include "GameEventManager.h"
#include <string>
#include <sstream>
#include <iostream>

Tower::Tower(int spawn_move) : spawn_move(spawn_move) {
    move_num = 0;
    ind_type = 'T';
    heallpoints = 30;
    enemyHealth = 10;
    enemyDamage = 10;

    LOG_INFO("Tower created with spawn move interval: " + std::to_string(spawn_move));
    LOG_DEBUG("Tower stats: HP=" + std::to_string(heallpoints) +
        ", EnemyHP=" + std::to_string(enemyHealth) +
        ", EnemyDamage=" + std::to_string(enemyDamage));
}

Tower::Tower(int spawn_move, int eHealth, int eDamage) : spawn_move(spawn_move) {
    move_num = 0;
    ind_type = 'T';
    heallpoints = 30;
    enemyHealth = eHealth;
    enemyDamage = eDamage;

    LOG_INFO("Tower created with custom enemy stats");
    LOG_DEBUG("Tower stats: HP=" + std::to_string(heallpoints) +
        ", EnemyHP=" + std::to_string(enemyHealth) +
        ", EnemyDamage=" + std::to_string(enemyDamage) +
        ", SpawnInterval=" + std::to_string(spawn_move));
}

void Tower::SetEnemyStats(int health, int damage) {
    int oldHealth = enemyHealth;
    int oldDamage = enemyDamage;

    enemyHealth = health;
    enemyDamage = damage;

    LOG_INFO("Tower enemy stats updated");
    LOG_DEBUG("Enemy stats: HP " + std::to_string(oldHealth) + "->" + std::to_string(enemyHealth) +
        ", Damage " + std::to_string(oldDamage) + "->" + std::to_string(enemyDamage));
}

void Tower::Slowed() {
    LOG_DEBUG("Tower slowed method called (no effect)");
}

void Tower::Spawn(int x, int y, Map& map) {
    this->x = x;
    this->y = y;
    map.SetVal(x, y, ind_type, this);

    LOG_INFO("Tower spawned at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    LOG_DEBUG("Tower HP: " + std::to_string(heallpoints) +
        ", Move number: " + std::to_string(move_num));
}

void Tower::Move(Map& map) {
    move_num++;
    LOG_DEBUG("Tower turn #" + std::to_string(move_num) +
        " at position (" + std::to_string(x) + "," + std::to_string(y) + ")");

    // Проверяем, нужно ли создавать врага
    if (move_num % spawn_move == 0) {
        int spawn_x = x - 1;
        int spawn_y = y;

        LOG_DEBUG("Attempting to spawn enemy at position (" +
            std::to_string(spawn_x) + "," + std::to_string(spawn_y) + ")");

        if (map.CanMoveTo(spawn_x, spawn_y)) {
            if (map.GetVal(spawn_x, spawn_y) == '0') {
                Enemy* enemy = new Enemy(enemyHealth, enemyDamage);
                enemy->Spawn(spawn_x, spawn_y, map);

                // Логирование создания врага через GameEventManager
                GameEventManager::getInstance().logTowerSpawnEnemy(x, y, enemyHealth);

                LOG_INFO("Tower spawned enemy with HP: " + std::to_string(enemyHealth) +
                    ", Damage: " + std::to_string(enemyDamage));
                LOG_DEBUG("Enemy spawned at position (" +
                    std::to_string(spawn_x) + "," + std::to_string(spawn_y) + ")");
            }
            else {
                char cell = map.GetVal(spawn_x, spawn_y);
                LOG_DEBUG("Cannot spawn enemy at (" +
                    std::to_string(spawn_x) + "," + std::to_string(spawn_y) +
                    ") - cell occupied by: '" + std::string(1, cell) + "'");
            }
        }
        else {
            LOG_DEBUG("Cannot spawn enemy at (" +
                std::to_string(spawn_x) + "," + std::to_string(spawn_y) +
                ") - cell not accessible");
        }
    }
    else {
        int turnsUntilSpawn = spawn_move - (move_num % spawn_move);
        LOG_DEBUG("Next enemy spawn in " + std::to_string(turnsUntilSpawn) + " turns");
    }
}

std::vector<uint8_t> Tower::Save() const {
    std::vector<uint8_t> data(SAVE_SIZE);
    size_t offset = 0;

    memcpy(data.data() + offset, &x, sizeof(x));
    offset += sizeof(x);

    memcpy(data.data() + offset, &y, sizeof(y));
    offset += sizeof(y);

    memcpy(data.data() + offset, &heallpoints, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(data.data() + offset, &move_num, sizeof(move_num));

    // Логирование сохранения башни
    LOG_DEBUG("Tower saved. Position: (" + std::to_string(x) + "," +
        std::to_string(y) + "), HP: " + std::to_string(heallpoints) +
        ", Move: " + std::to_string(move_num) +
        ", Size: " + std::to_string(SAVE_SIZE) + " bytes");

    return data;
}

void Tower::Load(const std::vector<uint8_t>& data) {
    if (data.size() != SAVE_SIZE) {
        LOG_ERROR("Invalid tower save data size: " + std::to_string(data.size()) +
            " (expected: " + std::to_string(SAVE_SIZE) + ")");
        return;
    }

    size_t offset = 0;

    int oldX = x, oldY = y, oldHP = heallpoints, oldMoveNum = move_num;

    memcpy(&x, data.data() + offset, sizeof(x));
    offset += sizeof(x);

    memcpy(&y, data.data() + offset, sizeof(y));
    offset += sizeof(y);

    memcpy(&heallpoints, data.data() + offset, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(&move_num, data.data() + offset, sizeof(move_num));

    // Логирование загрузки башни
    LOG_INFO("Tower loaded from save:");
    LOG_INFO("  Position: (" + std::to_string(oldX) + "," + std::to_string(oldY) +
        ") -> (" + std::to_string(x) + "," + std::to_string(y) + ")");
    LOG_INFO("  HP: " + std::to_string(oldHP) + " -> " + std::to_string(heallpoints));
    LOG_INFO("  Move number: " + std::to_string(oldMoveNum) + " -> " + std::to_string(move_num));

    // Проверка корректности загруженных данных
    if (heallpoints <= 0) {
        LOG_WARNING("Tower loaded with invalid HP (" + std::to_string(heallpoints) +
            "), resetting to 30");
        heallpoints = 30;
    }
}

size_t Tower::GetSaveSize() const {
    return SAVE_SIZE;
}

std::string Tower::SaveToString() const {
    std::ostringstream ss;
    ss << Entity::SaveToString();

    ss << "spawn_move:" << spawn_move << "\n";
    ss << "move_num:" << move_num << "\n";
    ss << "enemyHealth:" << enemyHealth << "\n";
    ss << "enemyDamage:" << enemyDamage << "\n";

    // Логирование создания строки сохранения
    std::string result = ss.str();
    LOG_DEBUG("Tower save string created. Length: " + std::to_string(result.length()) +
        " chars, position: (" + std::to_string(x) + "," + std::to_string(y) + ")");

    return ss.str();
}

void Tower::LoadFromString(const std::string& data) {
    LOG_DEBUG("Loading tower from string data. Length: " + std::to_string(data.length()));

    Entity::LoadFromString(data);

    std::istringstream ss(data);
    std::string line;

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        if (key == "spawn_move") {
            spawn_move = std::stoi(value);
            LOG_DEBUG("Loaded spawn_move: " + value);
        }
        else if (key == "move_num") {
            move_num = std::stoi(value);
            LOG_DEBUG("Loaded move_num: " + value);
        }
        else if (key == "enemyHealth") {
            enemyHealth = std::stoi(value);
            LOG_DEBUG("Loaded enemyHealth: " + value);
        }
        else if (key == "enemyDamage") {
            enemyDamage = std::stoi(value);
            LOG_DEBUG("Loaded enemyDamage: " + value);
        }
    }

    // Логирование результатов загрузки
    LOG_INFO("Tower loaded from string:");
    LOG_INFO("  Position: (" + std::to_string(x) + "," + std::to_string(y) + ")");
    LOG_INFO("  HP: " + std::to_string(heallpoints));
    LOG_INFO("  Spawn move interval: " + std::to_string(spawn_move));
    LOG_INFO("  Move number: " + std::to_string(move_num));
    LOG_INFO("  Enemy stats: HP=" + std::to_string(enemyHealth) +
        ", Damage=" + std::to_string(enemyDamage));

    // Проверка корректности данных
    if (heallpoints <= 0) {
        LOG_WARNING("Tower has invalid HP (" + std::to_string(heallpoints) +
            "), resetting to 30");
        heallpoints = 30;
    }

    if (spawn_move <= 0) {
        LOG_WARNING("Tower spawn move interval invalid (" + std::to_string(spawn_move) +
            "), resetting to 3");
        spawn_move = 3;
    }
}

std::string Tower::GetSaveHeader() const {
    return "TOWER";
}
