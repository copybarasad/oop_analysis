#include "Enemy.h"
#include "Map.h"  
#include "GameEventManager.h"
#include <iostream>
#include <cstring>
#include <sstream>

const char Enemy::traffic_map[4] = { 'l', 'l', 'r', 'r' };

Enemy::Enemy() {
    ind_type = 'e';
    heallpoints = 10;
    damage = 10;
    is_slowed = false;
    traffic_ind = 0;

    LOG_INFO("Enemy created with default stats");
}

Enemy::Enemy(int health, int dmg) {
    ind_type = 'e';
    heallpoints = health;
    damage = dmg;
    is_slowed = false;
    traffic_ind = 0;

    LOG_INFO("Enemy created with custom stats: HP=" + std::to_string(health) +
        ", Damage=" + std::to_string(dmg));
}

void Enemy::Slowed() {
    int oldHP = heallpoints;
    heallpoints -= 10;
    is_slowed = true;

    LOG_WARNING("Enemy slowed and took 10 trap damage");

    if (heallpoints <= 0) {
        LOG_INFO("Enemy defeated by trap damage");
        GameEventManager::getInstance().logEnemyDefeated(x, y);
    }
}

bool Enemy::IsSlowed() {
    if (is_slowed) {
        is_slowed = false;
        LOG_DEBUG("Enemy slow effect removed");
        return true;
    }
    return false;
}

void Enemy::Spawn(int x, int y, Map& map) {
    this->x = x;
    this->y = y;
    map.SetVal(x, y, ind_type, this);

    LOG_INFO("Enemy spawned at position (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    GameEventManager::getInstance().logEnemySpawned(x, y, heallpoints);
}

void Enemy::Move(Map& map) {
    if (heallpoints <= 0) {
        LOG_DEBUG("Enemy is dead, removing from map");
        map.SetVal(x, y, '0', nullptr);
        return;
    }

    if (IsSlowed()) {
        std::cout << "Enemy is slowed and skips turn!" << std::endl;
        LOG_INFO("Enemy is slowed, skipping turn");
        return;
    }

    int oldX = x;
    int oldY = y;

    int dx, dy;
    char direction = traffic_map[traffic_ind];
    GetDirection(direction, dx, dy);

    int new_x = x + dx;
    int new_y = y + dy;

    if (map.CanMoveTo(new_x, new_y)) {
        map.SetVal(x, y, '0', this);

        SetCoord(new_x, new_y);

        map.CheckTraps(new_x, new_y, this);

        map.SetVal(new_x, new_y, ind_type, this);

        GameEventManager::getInstance().logEnemyMoved(oldX, oldY, new_x, new_y);
        LOG_INFO("Enemy moved: (" + std::to_string(oldX) + "," + std::to_string(oldY) +
            ") -> (" + std::to_string(new_x) + "," + std::to_string(new_y) + ")");

        traffic_ind = (traffic_ind + 1) % 4;
    }
    else {
        if (map.GetVal(new_x, new_y) == 'H') {
            auto& objects = map.GetObjects();
            auto it = objects.find({ new_x, new_y });
            if (it != objects.end()) {
                Entity* hero = it->second;
                int oldHeroHP = hero->heallpoints;
                hero->heallpoints -= damage;

                GameEventManager::getInstance().logEnemyAttack(damage);
                LOG_WARNING("Enemy attacked player for " + std::to_string(damage) + " damage");
                LOG_DEBUG("Player HP: " + std::to_string(oldHeroHP) +
                    " -> " + std::to_string(hero->heallpoints));

                if (hero->heallpoints <= 0) {
                    LOG_CRITICAL("Player defeated by enemy");
                }
            }
        }

        traffic_ind = (traffic_ind + 1) % 4;
    }
}

std::vector<uint8_t> Enemy::Save() const {
    std::vector<uint8_t> data(SAVE_SIZE);
    size_t offset = 0;

    memcpy(data.data() + offset, &x, sizeof(x));
    offset += sizeof(x);

    memcpy(data.data() + offset, &y, sizeof(y));
    offset += sizeof(y);

    memcpy(data.data() + offset, &heallpoints, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(data.data() + offset, &is_slowed, sizeof(is_slowed));
    offset += sizeof(is_slowed);

    memcpy(data.data() + offset, &traffic_ind, sizeof(traffic_ind));

    LOG_DEBUG("Enemy saved. Position: (" + std::to_string(x) + "," +
        std::to_string(y) + "), HP: " + std::to_string(heallpoints));

    return data;
}

void Enemy::Load(const std::vector<uint8_t>& data) {
    if (data.size() != SAVE_SIZE) {
        LOG_ERROR("Invalid enemy save data size");
        return;
    }

    size_t offset = 0;

    memcpy(&x, data.data() + offset, sizeof(x));
    offset += sizeof(x);

    memcpy(&y, data.data() + offset, sizeof(y));
    offset += sizeof(y);

    memcpy(&heallpoints, data.data() + offset, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(&is_slowed, data.data() + offset, sizeof(is_slowed));
    offset += sizeof(is_slowed);

    memcpy(&traffic_ind, data.data() + offset, sizeof(traffic_ind));

    LOG_INFO("Enemy loaded from save. Position: (" + std::to_string(x) +
        "," + std::to_string(y) + "), HP: " + std::to_string(heallpoints));
}

size_t Enemy::GetSaveSize() const {
    return SAVE_SIZE;
}

std::string Enemy::SaveToString() const {
    std::ostringstream ss;
    ss << Entity::SaveToString();

    ss << "is_slowed:" << (is_slowed ? "1" : "0") << "\n";
    ss << "traffic_ind:" << traffic_ind << "\n";
    ss << "damage:" << damage << "\n";

    LOG_DEBUG("Enemy save string created");

    return ss.str();
}

void Enemy::LoadFromString(const std::string& data) {
    LOG_DEBUG("Loading enemy from string data");

    Entity::LoadFromString(data);

    std::istringstream ss(data);
    std::string line;

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        if (key == "is_slowed") {
            is_slowed = (value == "1");
        }
        else if (key == "traffic_ind") {
            traffic_ind = std::stoi(value);
        }
        else if (key == "damage") {
            damage = std::stoi(value);
        }
    }

    LOG_INFO("Enemy loaded from string. HP: " + std::to_string(heallpoints) +
        ", Damage: " + std::to_string(damage));
}

std::string Enemy::GetSaveHeader() const {
    return "ENEMY";
}

void Enemy::LogCurrentState() const {
    LOG_INFO("=== ENEMY STATE ===");
    LOG_INFO("Position: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
    LOG_INFO("Health: " + std::to_string(heallpoints));
    LOG_INFO("Damage: " + std::to_string(damage));
    LOG_INFO("Slowed: " + std::string(is_slowed ? "Yes" : "No"));
    LOG_INFO("Traffic index: " + std::to_string(traffic_ind));
    LOG_INFO("===================");
}

std::string Enemy::GetDebugInfo() const {
    std::stringstream info;
    info << "Enemy@" << x << "," << y
        << " HP:" << heallpoints
        << " DMG:" << damage
        << " SLOW:" << (is_slowed ? "Y" : "N");
    return info.str();
}

void Enemy::TakeDamage(int dmg, Map& map) {
    int oldHP = heallpoints;

    LOG_DEBUG("Enemy taking " + std::to_string(dmg) + " damage");

    Entity::TakeDamage(dmg, map);

    LOG_INFO("Enemy HP: " + std::to_string(oldHP) + " -> " + std::to_string(heallpoints));

    if (heallpoints <= 0) {
        LOG_INFO("Enemy defeated");
        GameEventManager::getInstance().logEnemyDefeated(x, y);
    }
}