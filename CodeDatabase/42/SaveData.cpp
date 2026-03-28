#include "SaveData.h"
#include <cstring>
#include <stdexcept>

size_t SaveData::getSerializedSize() const {
    size_t size = sizeof(version) + sizeof(fieldWidth) + 
                  sizeof(fieldHeight) + sizeof(playerPosition) + sizeof(playerHealth) +
                  sizeof(playerMaxHealth) + sizeof(playerAttackType) + 
                  sizeof(enemiesKilled) + sizeof(enemiesKilledSinceLastSpell) +
                  sizeof(turnCount);
    
    size += sizeof(size_t) + spells.size() * sizeof(SavedSpellType);
    
    size += sizeof(size_t) + enemies.size() * sizeof(SavedEnemy);
    
    size += sizeof(size_t) + towers.size() * sizeof(SavedTower);
    
    size += sizeof(size_t) + traps.size() * sizeof(SavedTrap);
    
    size += sizeof(size_t) + obstacles.size() * sizeof(SavedObstacle);
    
    return size;
}

void SaveData::serialize(char* buffer) const {
    char* ptr = buffer;
    
    memcpy(ptr, &version, sizeof(version));
    ptr += sizeof(version);
    
    memcpy(ptr, &fieldWidth, sizeof(fieldWidth));
    ptr += sizeof(fieldWidth);
    
    memcpy(ptr, &fieldHeight, sizeof(fieldHeight));
    ptr += sizeof(fieldHeight);
    
    memcpy(ptr, &playerPosition, sizeof(playerPosition));
    ptr += sizeof(playerPosition);
    
    memcpy(ptr, &playerHealth, sizeof(playerHealth));
    ptr += sizeof(playerHealth);
    
    memcpy(ptr, &playerMaxHealth, sizeof(playerMaxHealth));
    ptr += sizeof(playerMaxHealth);
    
    memcpy(ptr, &playerAttackType, sizeof(playerAttackType));
    ptr += sizeof(playerAttackType);
    
    memcpy(ptr, &enemiesKilled, sizeof(enemiesKilled));
    ptr += sizeof(enemiesKilled);
    
    memcpy(ptr, &enemiesKilledSinceLastSpell, sizeof(enemiesKilledSinceLastSpell));
    ptr += sizeof(enemiesKilledSinceLastSpell);
    
    memcpy(ptr, &turnCount, sizeof(turnCount));
    ptr += sizeof(turnCount);
    
    size_t spellCount = spells.size();
    memcpy(ptr, &spellCount, sizeof(spellCount));
    ptr += sizeof(spellCount);
    
    for (const auto& spell : spells) {
        memcpy(ptr, &spell, sizeof(spell));
        ptr += sizeof(spell);
    }
    
    size_t enemyCount = enemies.size();
    memcpy(ptr, &enemyCount, sizeof(enemyCount));
    ptr += sizeof(enemyCount);
    
    for (const auto& enemy : enemies) {
        memcpy(ptr, &enemy, sizeof(enemy));
        ptr += sizeof(enemy);
    }
    
    size_t towerCount = towers.size();
    memcpy(ptr, &towerCount, sizeof(towerCount));
    ptr += sizeof(towerCount);
    
    for (const auto& tower : towers) {
        memcpy(ptr, &tower, sizeof(tower));
        ptr += sizeof(tower);
    }
    
    size_t trapCount = traps.size();
    memcpy(ptr, &trapCount, sizeof(trapCount));
    ptr += sizeof(trapCount);
    
    for (const auto& trap : traps) {
        memcpy(ptr, &trap, sizeof(trap));
        ptr += sizeof(trap);
    }
    
    size_t obstacleCount = obstacles.size();
    memcpy(ptr, &obstacleCount, sizeof(obstacleCount));
    ptr += sizeof(obstacleCount);
    
    for (const auto& obstacle : obstacles) {
        memcpy(ptr, &obstacle, sizeof(obstacle));
        ptr += sizeof(obstacle);
    }
}

bool SaveData::deserialize(const char* buffer, size_t size) {
    const char* ptr = buffer;
    
    int readVersion;
    if (ptr + sizeof(readVersion) > buffer + size) return false;
    memcpy(&readVersion, ptr, sizeof(readVersion));
    ptr += sizeof(readVersion);
    
    if (readVersion != SAVE_VERSION) {
        throw std::runtime_error("Unsupported save version: " + std::to_string(readVersion));
    }
    
    if (ptr + sizeof(fieldWidth) + sizeof(fieldHeight) + 
        sizeof(playerPosition) + sizeof(playerHealth) + sizeof(playerMaxHealth) +
        sizeof(playerAttackType) + sizeof(enemiesKilled) + sizeof(enemiesKilledSinceLastSpell) +
        sizeof(turnCount) > buffer + size) return false;
    
    memcpy(&fieldWidth, ptr, sizeof(fieldWidth));
    ptr += sizeof(fieldWidth);
    
    memcpy(&fieldHeight, ptr, sizeof(fieldHeight));
    ptr += sizeof(fieldHeight);
    
    memcpy(&playerPosition, ptr, sizeof(playerPosition));
    ptr += sizeof(playerPosition);
    
    memcpy(&playerHealth, ptr, sizeof(playerHealth));
    ptr += sizeof(playerHealth);
    
    memcpy(&playerMaxHealth, ptr, sizeof(playerMaxHealth));
    ptr += sizeof(playerMaxHealth);
    
    memcpy(&playerAttackType, ptr, sizeof(playerAttackType));
    ptr += sizeof(playerAttackType);
    
    memcpy(&enemiesKilled, ptr, sizeof(enemiesKilled));
    ptr += sizeof(enemiesKilled);
    
    memcpy(&enemiesKilledSinceLastSpell, ptr, sizeof(enemiesKilledSinceLastSpell));
    ptr += sizeof(enemiesKilledSinceLastSpell);
    
    memcpy(&turnCount, ptr, sizeof(turnCount));
    ptr += sizeof(turnCount);
    
    size_t spellCount;
    if (ptr + sizeof(spellCount) > buffer + size) return false;
    memcpy(&spellCount, ptr, sizeof(spellCount));
    ptr += sizeof(spellCount);
    
    spells.resize(spellCount);
    if (ptr + spellCount * sizeof(SavedSpellType) > buffer + size) return false;
    for (size_t i = 0; i < spellCount; i++) {
        memcpy(&spells[i], ptr, sizeof(SavedSpellType));
        ptr += sizeof(SavedSpellType);
    }
    
    size_t enemyCount;
    if (ptr + sizeof(enemyCount) > buffer + size) return false;
    memcpy(&enemyCount, ptr, sizeof(enemyCount));
    ptr += sizeof(enemyCount);
    
    enemies.resize(enemyCount);
    if (ptr + enemyCount * sizeof(SavedEnemy) > buffer + size) return false;
    for (size_t i = 0; i < enemyCount; i++) {
        memcpy(&enemies[i], ptr, sizeof(SavedEnemy));
        ptr += sizeof(SavedEnemy);
    }
    
    size_t towerCount;
    if (ptr + sizeof(towerCount) > buffer + size) return false;
    memcpy(&towerCount, ptr, sizeof(towerCount));
    ptr += sizeof(towerCount);
    
    towers.resize(towerCount);
    if (ptr + towerCount * sizeof(SavedTower) > buffer + size) return false;
    for (size_t i = 0; i < towerCount; i++) {
        memcpy(&towers[i], ptr, sizeof(SavedTower));
        ptr += sizeof(SavedTower);
    }
    
    size_t trapCount;
    if (ptr + sizeof(trapCount) > buffer + size) return false;
    memcpy(&trapCount, ptr, sizeof(trapCount));
    ptr += sizeof(trapCount);
    
    traps.resize(trapCount);
    if (ptr + trapCount * sizeof(SavedTrap) > buffer + size) return false;
    for (size_t i = 0; i < trapCount; i++) {
        memcpy(&traps[i], ptr, sizeof(SavedTrap));
        ptr += sizeof(SavedTrap);
    }
    
    size_t obstacleCount;
    if (ptr + sizeof(obstacleCount) > buffer + size) return false;
    memcpy(&obstacleCount, ptr, sizeof(obstacleCount));
    ptr += sizeof(obstacleCount);
    
    obstacles.resize(obstacleCount);
    if (ptr + obstacleCount * sizeof(SavedObstacle) > buffer + size) return false;
    for (size_t i = 0; i < obstacleCount; i++) {
        memcpy(&obstacles[i], ptr, sizeof(SavedObstacle));
        ptr += sizeof(SavedObstacle);
    }
    
    return true;
}