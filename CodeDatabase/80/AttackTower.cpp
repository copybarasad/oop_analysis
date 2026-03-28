#include "AttackTower.h"
#include "Map.h"
#include "Player.h"
#include "GameEventManager.h" 
#include <cmath>
#include <iostream>
#include <cstring>
#include <sstream>

AttackTower::AttackTower() {
    ind_type = 'B';
    heallpoints = 50;
    damage = 15;
    attack_range = 5;
    attack_cooldown = 3;
    current_cooldown = 0;

    attack_spell = std::make_unique<SpellDirectDamage>(damage / 2, attack_range);

    LOG_INFO("Attack Tower created");
    LOG_DEBUG("Attack Tower stats: HP=" + std::to_string(heallpoints) +
        ", Damage=" + std::to_string(damage));
}

void AttackTower::Slowed() {
    LOG_DEBUG("Attack Tower Slowed() called");
}

void AttackTower::Spawn(int x, int y, Map& map) {
    this->x = x;
    this->y = y;
    map.SetVal(x, y, ind_type, this);

    LOG_INFO("Attack Tower spawned at (" + std::to_string(x) + "," + std::to_string(y) + ")");
}

void AttackTower::Move(Map& map) {
    LOG_DEBUG("Attack Tower Move() called");

    AttackIfPlayerInRange(map);
}

void AttackTower::AttackIfPlayerInRange(Map& map) {
    LOG_DEBUG("Attack Tower checking for player in range");

    if (current_cooldown > 0) {
        current_cooldown--;
        LOG_DEBUG("Attack Tower on cooldown: " + std::to_string(current_cooldown) + " turns left");
        return;
    }

    auto& objects = map.GetObjects();
    for (auto& [coord, entity] : objects) {
        if (entity && entity->ind_type == 'H') {
            int player_x = entity->x;
            int player_y = entity->y;

            double distance = sqrt(pow(x - player_x, 2) + pow(y - player_y, 2));

            LOG_DEBUG("Player at (" + std::to_string(player_x) + "," + std::to_string(player_y) +
                "), distance: " + std::to_string(distance));

            if (distance <= attack_range) {
                if (attack_spell->CanCast(x, y, player_x, player_y, map)) {
                    LOG_INFO("Attack Tower casting spell on player");

                    attack_spell->UseSpell(player_x, player_y, map);
                    std::cout << "Enemy tower casts spell on player!" << std::endl;

                    GameEventManager::getInstance().logTowerAttack(damage / 2);

                    LOG_DEBUG("Attack Tower cooldown set to " + std::to_string(attack_cooldown));

                    current_cooldown = attack_cooldown;
                    break;
                }
                else {
                    LOG_DEBUG("Cannot cast spell - line of sight blocked");
                }
            }
            else {
                LOG_DEBUG("Player out of range (distance: " + std::to_string(distance) +
                    ", range: " + std::to_string(attack_range) + ")");
            }
        }
    }
}

std::vector<uint8_t> AttackTower::Save() const {
    LOG_DEBUG("Attack Tower Save() called");

    std::vector<uint8_t> data(SAVE_SIZE);
    size_t offset = 0;

    memcpy(data.data() + offset, &x, sizeof(x));
    offset += sizeof(x);

    memcpy(data.data() + offset, &y, sizeof(y));
    offset += sizeof(y);

    memcpy(data.data() + offset, &heallpoints, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(data.data() + offset, &current_cooldown, sizeof(current_cooldown));

    LOG_DEBUG("Attack Tower saved: HP=" + std::to_string(heallpoints) +
        ", cooldown=" + std::to_string(current_cooldown));

    return data;
}

void AttackTower::Load(const std::vector<uint8_t>& data) {
    LOG_DEBUG("Attack Tower Load() called, data size: " + std::to_string(data.size()));

    if (data.size() != SAVE_SIZE) {
        LOG_ERROR("Attack Tower Load() failed - invalid data size");
        return;
    }

    size_t offset = 0;

    memcpy(&x, data.data() + offset, sizeof(x));
    offset += sizeof(x);

    memcpy(&y, data.data() + offset, sizeof(y));
    offset += sizeof(y);

    memcpy(&heallpoints, data.data() + offset, sizeof(heallpoints));
    offset += sizeof(heallpoints);

    memcpy(&current_cooldown, data.data() + offset, sizeof(current_cooldown));

    LOG_INFO("Attack Tower loaded: position=(" + std::to_string(x) + "," + std::to_string(y) +
        "), HP=" + std::to_string(heallpoints));
}

size_t AttackTower::GetSaveSize() const {
    LOG_DEBUG("Attack Tower GetSaveSize() called: " + std::to_string(SAVE_SIZE) + " bytes");
    return SAVE_SIZE;
}

std::string AttackTower::SaveToString() const {
    LOG_DEBUG("Attack Tower SaveToString() called");

    std::ostringstream ss;
    ss << Entity::SaveToString();

    ss << "damage:" << damage << "\n";
    ss << "attack_range:" << attack_range << "\n";
    ss << "attack_cooldown:" << attack_cooldown << "\n";
    ss << "current_cooldown:" << current_cooldown << "\n";

    std::string result = ss.str();
    LOG_DEBUG("Attack Tower save string length: " + std::to_string(result.length()) + " chars");

    return ss.str();
}

void AttackTower::LoadFromString(const std::string& data) {
    LOG_DEBUG("Attack Tower LoadFromString() called, data length: " + std::to_string(data.length()));

    Entity::LoadFromString(data);

    std::istringstream ss(data);
    std::string line;

    damage = 15;
    attack_range = 5;
    attack_cooldown = 3;
    current_cooldown = 0;

    while (std::getline(ss, line)) {
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) continue;

        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        if (key == "damage") {
            damage = std::stoi(value);
            LOG_DEBUG("Loaded damage: " + value);
        }
        else if (key == "attack_range") {
            attack_range = std::stoi(value);
            LOG_DEBUG("Loaded attack_range: " + value);
        }
        else if (key == "attack_cooldown") {
            attack_cooldown = std::stoi(value);
            LOG_DEBUG("Loaded attack_cooldown: " + value);
        }
        else if (key == "current_cooldown") {
            current_cooldown = std::stoi(value);
            LOG_DEBUG("Loaded current_cooldown: " + value);
        }
    }

    attack_spell = std::make_unique<SpellDirectDamage>(damage / 2, attack_range);

    LOG_INFO("Attack Tower loaded from string: damage=" + std::to_string(damage) +
        ", range=" + std::to_string(attack_range));

    std::cout << "DEBUG - AttackTower loaded: x=" << x << ", y=" << y
        << ", damage=" << damage << ", range=" << attack_range << std::endl;
}

std::string AttackTower::GetSaveHeader() const {
    LOG_DEBUG("Attack Tower GetSaveHeader() called: ATTACK_TOWER");
    return "ATTACK_TOWER";
}