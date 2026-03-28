#include "GameState.h"
#include "GameExceptions.h"
#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include <fstream>
#include <filesystem>

// ============================================================================
// Реализация EnemyState
// ============================================================================

EnemyState::EnemyState(const Enemy& enemy)
    : x(enemy.GetX())
    , y(enemy.GetY())
    , health(enemy.GetHealth())
    , damage(enemy.GetBaseDamage()) {
}

Enemy EnemyState::ToEnemy() const {
    Enemy enemy(health, damage);
    enemy.SetPosition(x, y);
    return enemy;
}

// ============================================================================
// Реализация BuildingState
// ============================================================================

BuildingState::BuildingState(const EnemyBuilding& building)
    : x(building.GetX())
    , y(building.GetY())
    , spawnInterval(building.GetSpawnInterval())
    , currentCounter(building.GetCurrentCounter())
    , health(building.GetHealth()) {
}

EnemyBuilding BuildingState::ToBuilding() const {
    EnemyBuilding building(spawnInterval, x, y, health);
    // Устанавливаем текущий счётчик через Update
    int turnsToSkip = spawnInterval - currentCounter;
    for (int i = 0; i < turnsToSkip; ++i) {
        building.Update();
    }
    return building;
}

// ============================================================================
// Реализация TowerState
// ============================================================================

TowerState::TowerState(const EnemyTower& tower)
    : x(tower.GetX())
    , y(tower.GetY())
    , attackRadius(tower.GetAttackRadius())
    , damage(tower.GetDamage())
    , cooldownTurns(tower.GetCooldownTurns())
    , currentCooldown(tower.GetCurrentCooldown()) {
}

EnemyTower TowerState::ToTower() const {
    return EnemyTower(x, y, attackRadius, damage, cooldownTurns);
}

// ============================================================================
// Реализация AllyState
// ============================================================================

AllyState::AllyState(const Ally& ally)
    : x(ally.GetX())
    , y(ally.GetY())
    , health(ally.GetHealth())
    , damage(ally.GetDamage()) {
}

Ally AllyState::ToAlly() const {
    return Ally(x, y, health, damage);
}

// ============================================================================
// Реализация TrapState
// ============================================================================

TrapState::TrapState(const Trap& trap)
    : x(trap.GetX())
    , y(trap.GetY())
    , damage(trap.GetDamage())
    , triggered(trap.IsTriggered()) {
}

Trap TrapState::ToTrap() const {
    Trap trap(x, y, damage);
    if (triggered) {
        trap.Trigger();
    }
    return trap;
}

// ============================================================================
// Реализация SpellState
// ============================================================================

SpellState::SpellState(const Spell* spell) {
    if (!spell) {
        type = "None";
        baseDamage = currentDamage = baseRadius = currentRadius = extraData = 0;
        return;
    }
    
    std::string name = spell->GetName();
    
    if (name == "Direct Damage") {
        type = "DirectDamage";
    } else if (name == "Area Damage") {
        type = "AreaDamage";
    } else if (name == "Trap") {
        type = "Trap";
    } else if (name == "Summon") {
        type = "Summon";
    } else if (name == "Enhancement") {
        type = "Enhancement";
    } else {
        type = "Unknown";
    }
    
    // Заглушка: идеально нужны геттеры в Spell, но для простоты используем значения по умолчанию
    baseDamage = 50;
    currentDamage = 50;
    baseRadius = 5;
    currentRadius = 5;
    extraData = 0;
}

std::unique_ptr<Spell> SpellState::ToSpell() const {
    if (type == "DirectDamage") {
        return std::make_unique<DirectDamageSpell>(baseDamage, baseRadius);
    } else if (type == "AreaDamage") {
        return std::make_unique<AreaDamageSpell>(baseDamage, baseRadius, 2);
    } else if (type == "Trap") {
        return std::make_unique<TrapSpell>(baseDamage, baseRadius);
    } else if (type == "Summon") {
        return std::make_unique<SummonSpell>(1, baseRadius);
    } else if (type == "Enhancement") {
        return std::make_unique<EnhancementSpell>();
    }
    return nullptr;
}

// ============================================================================
// Реализация GameState - Конструкторы
// ============================================================================

GameState::GameState(const Player& player, const GameField& field, 
                     const LevelManager& levelManager, int turn) {
    capturePlayerState(player);
    captureFieldState(field);
    captureLevelState(levelManager, turn);
}

GameState GameState::CaptureFromGame(const Player& player, const GameField& field,
                                     const LevelManager& levelManager, int turn) {
    GameState state;
    state.capturePlayerState(player);
    
    // Захватываем состояние поля
    state.fieldWidth = field.GetWidth();
    state.fieldHeight = field.GetHeight();
    state.playerX = field.GetPlayerX();
    state.playerY = field.GetPlayerY();
    
    // Захватываем врагов
    for (size_t i = 0; i < field.GetEnemyCount(); ++i) {
        state.enemies.push_back(EnemyState(field.GetEnemyByIndex(i)));
    }
    
    // Захватываем здания
    for (size_t i = 0; i < field.GetBuildingCount(); ++i) {
        state.buildings.push_back(BuildingState(field.GetBuildingByIndex(i)));
    }
    
    // Захватываем башни
    for (size_t i = 0; i < field.GetTowerCount(); ++i) {
        state.towers.push_back(TowerState(field.GetTowerByIndex(i)));
    }
    
    // Захватываем союзников
    for (size_t i = 0; i < field.GetAllyCount(); ++i) {
        state.allies.push_back(AllyState(field.GetAllyByIndex(i)));
    }
    
    // Захватываем ловушки
    for (size_t i = 0; i < field.GetTrapCount(); ++i) {
        state.traps.push_back(TrapState(field.GetTrapByIndex(i)));
    }
    
    state.captureLevelState(levelManager, turn);
    
    return state;
}

// ============================================================================
// Реализация GameState - Захват состояния
// ============================================================================

void GameState::capturePlayerState(const Player& player) {
    playerHealth = player.GetHealth();
    playerMaxHealth = player.GetMaxHealth();
    playerScore = player.GetScore();
    playerCombatMode = player.GetCombatMode();
    playerIsSlowed = !player.CanMove();
    
    // Сохраняем заклинания
    playerSpells.clear();
    for (size_t i = 0; i < player.GetSpellCount(); ++i) {
        Spell* spell = const_cast<Player&>(player).GetSpellAt(i);
        if (spell) {
            playerSpells.push_back(SpellState(spell));
        }
    }
}

void GameState::captureFieldState(const GameField& field) {
    fieldWidth = field.GetWidth();
    fieldHeight = field.GetHeight();
    playerX = field.GetPlayerX();
    playerY = field.GetPlayerY();
    
    // Примечание: клетки поля не сохраняем для упрощения
    // В реальной реализации нужно добавить GetCellType в FieldGrid
}

void GameState::captureLevelState(const LevelManager& levelManager, int turn) {
    currentLevel = levelManager.GetCurrentLevelNumber();
    currentTurn = turn;
}

// ============================================================================
// Реализация GameState - Применение состояния
// ============================================================================

void GameState::ApplyToGame(Player& player, GameField& field, 
                            LevelManager& levelManager, int& turn) const {
    restorePlayerState(player);
    restoreFieldState(field);
    restoreLevelState(levelManager, turn);
}

void GameState::restorePlayerState(Player& player) const {
    player.SetHealth(playerHealth);
    
    int currentScore = player.GetScore();
    if (playerScore > currentScore) {
        player.AddScore(playerScore - currentScore);
    }
    
    // Восстанавливаем заклинания
    // Очищаем текущие
    while (player.GetSpellCount() > 0) {
        player.RemoveSpellAt(0);
    }
    
    // Добавляем сохранённые
    for (const auto& spellState : playerSpells) {
        auto spell = spellState.ToSpell();
        if (spell) {
            player.AddSpell(std::move(spell));
        }
    }
}

void GameState::restoreFieldState(GameField& field) const {
    field.SetPlayerPosition(playerX, playerY);
    
    // Восстанавливаем сущности
    for (const auto& enemyState : enemies) {
        field.AddEnemy(enemyState.ToEnemy());
    }
    
    for (const auto& buildingState : buildings) {
        field.AddEnemyBuilding(buildingState.ToBuilding());
    }
    
    for (const auto& towerState : towers) {
        field.AddEnemyTower(towerState.ToTower());
    }
    
    for (const auto& allyState : allies) {
        field.AddAlly(allyState.ToAlly());
    }
    
    for (const auto& trapState : traps) {
        if (!trapState.triggered) {
            field.PlaceTrap(trapState.x, trapState.y, trapState.damage);
        }
    }
}

void GameState::restoreLevelState(LevelManager& levelManager, int& turn) const {
    levelManager.SetCurrentLevel(currentLevel);
    turn = currentTurn;
}

// ============================================================================
// Реализация GameState - Сериализация
// ============================================================================

void GameState::SaveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        throw SaveFileWriteException(filename);
    }
    
    try {
        // Магическое число
        unsigned int magic = 0x47534154;  // 'GSAT'
        file.write(reinterpret_cast<const char*>(&magic), sizeof(magic));
        
        // Версия
        int version = 2;
        file.write(reinterpret_cast<const char*>(&version), sizeof(version));
        
        // Состояние игрока
        file.write(reinterpret_cast<const char*>(&playerHealth), sizeof(playerHealth));
        file.write(reinterpret_cast<const char*>(&playerMaxHealth), sizeof(playerMaxHealth));
        file.write(reinterpret_cast<const char*>(&playerScore), sizeof(playerScore));
        file.write(reinterpret_cast<const char*>(&playerCombatMode), sizeof(playerCombatMode));
        file.write(reinterpret_cast<const char*>(&playerIsSlowed), sizeof(playerIsSlowed));
        
        // Заклинания
        size_t spellCount = playerSpells.size();
        file.write(reinterpret_cast<const char*>(&spellCount), sizeof(spellCount));
        for (const auto& spell : playerSpells) {
            size_t typeLen = spell.type.length();
            file.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            file.write(spell.type.c_str(), typeLen);
            file.write(reinterpret_cast<const char*>(&spell.baseDamage), sizeof(spell.baseDamage));
            file.write(reinterpret_cast<const char*>(&spell.currentDamage), sizeof(spell.currentDamage));
            file.write(reinterpret_cast<const char*>(&spell.baseRadius), sizeof(spell.baseRadius));
            file.write(reinterpret_cast<const char*>(&spell.currentRadius), sizeof(spell.currentRadius));
            file.write(reinterpret_cast<const char*>(&spell.extraData), sizeof(spell.extraData));
        }
        
        // Состояние поля
        file.write(reinterpret_cast<const char*>(&fieldWidth), sizeof(fieldWidth));
        file.write(reinterpret_cast<const char*>(&fieldHeight), sizeof(fieldHeight));
        file.write(reinterpret_cast<const char*>(&playerX), sizeof(playerX));
        file.write(reinterpret_cast<const char*>(&playerY), sizeof(playerY));
        
        // Враги
        size_t enemyCount = enemies.size();
        file.write(reinterpret_cast<const char*>(&enemyCount), sizeof(enemyCount));
        for (const auto& enemy : enemies) {
            file.write(reinterpret_cast<const char*>(&enemy.x), sizeof(enemy.x));
            file.write(reinterpret_cast<const char*>(&enemy.y), sizeof(enemy.y));
            file.write(reinterpret_cast<const char*>(&enemy.health), sizeof(enemy.health));
            file.write(reinterpret_cast<const char*>(&enemy.damage), sizeof(enemy.damage));
        }
        
        // Здания
        size_t buildingCount = buildings.size();
        file.write(reinterpret_cast<const char*>(&buildingCount), sizeof(buildingCount));
        for (const auto& building : buildings) {
            file.write(reinterpret_cast<const char*>(&building.x), sizeof(building.x));
            file.write(reinterpret_cast<const char*>(&building.y), sizeof(building.y));
            file.write(reinterpret_cast<const char*>(&building.spawnInterval), sizeof(building.spawnInterval));
            file.write(reinterpret_cast<const char*>(&building.currentCounter), sizeof(building.currentCounter));
            file.write(reinterpret_cast<const char*>(&building.health), sizeof(building.health));
        }
        
        // Башни
        size_t towerCount = towers.size();
        file.write(reinterpret_cast<const char*>(&towerCount), sizeof(towerCount));
        for (const auto& tower : towers) {
            file.write(reinterpret_cast<const char*>(&tower.x), sizeof(tower.x));
            file.write(reinterpret_cast<const char*>(&tower.y), sizeof(tower.y));
            file.write(reinterpret_cast<const char*>(&tower.attackRadius), sizeof(tower.attackRadius));
            file.write(reinterpret_cast<const char*>(&tower.damage), sizeof(tower.damage));
            file.write(reinterpret_cast<const char*>(&tower.cooldownTurns), sizeof(tower.cooldownTurns));
            file.write(reinterpret_cast<const char*>(&tower.currentCooldown), sizeof(tower.currentCooldown));
        }
        
        // Союзники
        size_t allyCount = allies.size();
        file.write(reinterpret_cast<const char*>(&allyCount), sizeof(allyCount));
        for (const auto& ally : allies) {
            file.write(reinterpret_cast<const char*>(&ally.x), sizeof(ally.x));
            file.write(reinterpret_cast<const char*>(&ally.y), sizeof(ally.y));
            file.write(reinterpret_cast<const char*>(&ally.health), sizeof(ally.health));
            file.write(reinterpret_cast<const char*>(&ally.damage), sizeof(ally.damage));
        }
        
        // Ловушки
        size_t trapCount = traps.size();
        file.write(reinterpret_cast<const char*>(&trapCount), sizeof(trapCount));
        for (const auto& trap : traps) {
            file.write(reinterpret_cast<const char*>(&trap.x), sizeof(trap.x));
            file.write(reinterpret_cast<const char*>(&trap.y), sizeof(trap.y));
            file.write(reinterpret_cast<const char*>(&trap.damage), sizeof(trap.damage));
            file.write(reinterpret_cast<const char*>(&trap.triggered), sizeof(trap.triggered));
        }
        
        // Состояние уровня
        file.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
        file.write(reinterpret_cast<const char*>(&currentTurn), sizeof(currentTurn));
        
        if (!file.good()) {
            throw SaveFileWriteException(filename);
        }
        
        file.close();
    } catch (const SaveFileException&) {
        file.close();
        throw;
    } catch (const std::exception&) {
        file.close();
        throw SaveFileWriteException(filename);
    }
}

GameState GameState::LoadFromFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        throw SaveFileNotFoundException(filename);
    }
    
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        throw SaveFileReadException(filename);
    }
    
    GameState state;
    
    try {
        // Проверяем магическое число
        unsigned int magic;
        file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
        if (magic != 0x47534154) {
            throw SaveFileCorruptedException(filename, "неверная сигнатура файла");
        }
        
        // Проверяем версию
        int version;
        file.read(reinterpret_cast<char*>(&version), sizeof(version));
        if (version != 2) {
            throw SaveFileCorruptedException(filename, "несовместимая версия");
        }
        
        // Состояние игрока
        file.read(reinterpret_cast<char*>(&state.playerHealth), sizeof(state.playerHealth));
        file.read(reinterpret_cast<char*>(&state.playerMaxHealth), sizeof(state.playerMaxHealth));
        file.read(reinterpret_cast<char*>(&state.playerScore), sizeof(state.playerScore));
        file.read(reinterpret_cast<char*>(&state.playerCombatMode), sizeof(state.playerCombatMode));
        file.read(reinterpret_cast<char*>(&state.playerIsSlowed), sizeof(state.playerIsSlowed));
        
        // Заклинания
        size_t spellCount;
        file.read(reinterpret_cast<char*>(&spellCount), sizeof(spellCount));
        state.playerSpells.reserve(spellCount);
        for (size_t i = 0; i < spellCount; ++i) {
            SpellState spell;
            size_t typeLen;
            file.read(reinterpret_cast<char*>(&typeLen), sizeof(typeLen));
            spell.type.resize(typeLen);
            file.read(&spell.type[0], typeLen);
            file.read(reinterpret_cast<char*>(&spell.baseDamage), sizeof(spell.baseDamage));
            file.read(reinterpret_cast<char*>(&spell.currentDamage), sizeof(spell.currentDamage));
            file.read(reinterpret_cast<char*>(&spell.baseRadius), sizeof(spell.baseRadius));
            file.read(reinterpret_cast<char*>(&spell.currentRadius), sizeof(spell.currentRadius));
            file.read(reinterpret_cast<char*>(&spell.extraData), sizeof(spell.extraData));
            state.playerSpells.push_back(spell);
        }
        
        // Состояние поля
        file.read(reinterpret_cast<char*>(&state.fieldWidth), sizeof(state.fieldWidth));
        file.read(reinterpret_cast<char*>(&state.fieldHeight), sizeof(state.fieldHeight));
        file.read(reinterpret_cast<char*>(&state.playerX), sizeof(state.playerX));
        file.read(reinterpret_cast<char*>(&state.playerY), sizeof(state.playerY));
        
        // Враги
        size_t enemyCount;
        file.read(reinterpret_cast<char*>(&enemyCount), sizeof(enemyCount));
        state.enemies.reserve(enemyCount);
        for (size_t i = 0; i < enemyCount; ++i) {
            EnemyState enemy;
            file.read(reinterpret_cast<char*>(&enemy.x), sizeof(enemy.x));
            file.read(reinterpret_cast<char*>(&enemy.y), sizeof(enemy.y));
            file.read(reinterpret_cast<char*>(&enemy.health), sizeof(enemy.health));
            file.read(reinterpret_cast<char*>(&enemy.damage), sizeof(enemy.damage));
            state.enemies.push_back(enemy);
        }
        
        // Здания
        size_t buildingCount;
        file.read(reinterpret_cast<char*>(&buildingCount), sizeof(buildingCount));
        state.buildings.reserve(buildingCount);
        for (size_t i = 0; i < buildingCount; ++i) {
            BuildingState building;
            file.read(reinterpret_cast<char*>(&building.x), sizeof(building.x));
            file.read(reinterpret_cast<char*>(&building.y), sizeof(building.y));
            file.read(reinterpret_cast<char*>(&building.spawnInterval), sizeof(building.spawnInterval));
            file.read(reinterpret_cast<char*>(&building.currentCounter), sizeof(building.currentCounter));
            file.read(reinterpret_cast<char*>(&building.health), sizeof(building.health));
            state.buildings.push_back(building);
        }
        
        // Башни
        size_t towerCount;
        file.read(reinterpret_cast<char*>(&towerCount), sizeof(towerCount));
        state.towers.reserve(towerCount);
        for (size_t i = 0; i < towerCount; ++i) {
            TowerState tower;
            file.read(reinterpret_cast<char*>(&tower.x), sizeof(tower.x));
            file.read(reinterpret_cast<char*>(&tower.y), sizeof(tower.y));
            file.read(reinterpret_cast<char*>(&tower.attackRadius), sizeof(tower.attackRadius));
            file.read(reinterpret_cast<char*>(&tower.damage), sizeof(tower.damage));
            file.read(reinterpret_cast<char*>(&tower.cooldownTurns), sizeof(tower.cooldownTurns));
            file.read(reinterpret_cast<char*>(&tower.currentCooldown), sizeof(tower.currentCooldown));
            state.towers.push_back(tower);
        }
        
        // Союзники
        size_t allyCount;
        file.read(reinterpret_cast<char*>(&allyCount), sizeof(allyCount));
        state.allies.reserve(allyCount);
        for (size_t i = 0; i < allyCount; ++i) {
            AllyState ally;
            file.read(reinterpret_cast<char*>(&ally.x), sizeof(ally.x));
            file.read(reinterpret_cast<char*>(&ally.y), sizeof(ally.y));
            file.read(reinterpret_cast<char*>(&ally.health), sizeof(ally.health));
            file.read(reinterpret_cast<char*>(&ally.damage), sizeof(ally.damage));
            state.allies.push_back(ally);
        }
        
        // Ловушки
        size_t trapCount;
        file.read(reinterpret_cast<char*>(&trapCount), sizeof(trapCount));
        state.traps.reserve(trapCount);
        for (size_t i = 0; i < trapCount; ++i) {
            TrapState trap;
            file.read(reinterpret_cast<char*>(&trap.x), sizeof(trap.x));
            file.read(reinterpret_cast<char*>(&trap.y), sizeof(trap.y));
            file.read(reinterpret_cast<char*>(&trap.damage), sizeof(trap.damage));
            file.read(reinterpret_cast<char*>(&trap.triggered), sizeof(trap.triggered));
            state.traps.push_back(trap);
        }
        
        // Состояние уровня
        file.read(reinterpret_cast<char*>(&state.currentLevel), sizeof(state.currentLevel));
        file.read(reinterpret_cast<char*>(&state.currentTurn), sizeof(state.currentTurn));
        
        if (!file.good() && !file.eof()) {
            throw SaveFileCorruptedException(filename, "ошибка чтения данных");
        }
        
        file.close();
        return state;
        
    } catch (const SaveFileException&) {
        file.close();
        throw;
    } catch (const std::exception& error) {
        file.close();
        throw SaveFileCorruptedException(filename, error.what());
    }
}

