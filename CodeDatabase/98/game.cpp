#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include "game.h"

Game::Game(int width, int height, double chance, int countenemy): 
    field(width, height), 
    hand(3), 
    chance(chance), 
    baseEnemyCount(countenemy),
    isLevelActive(false),
    lastAction("") {
    
    std::srand(std::time(nullptr));
    GenerateLevel(width, height, chance, countenemy);
    hand.AddRandomSpell();
}

void Game::GenerateLevel(int width, int height, double chance, int countenemy) {
    field = Field(width, height); 
    enemies.clear();
    enemyPositions.clear();

    field.AddBlockCell(chance);
    
    int freeCells = 0;
    for(int y = 0; y < field.GetHeight(); y++) {
        for(int x = 0; x < field.GetWidth(); x++) {
            if(!field.GetCell(x, y).IsBlocked()) freeCells++;
        }
    }

    if(countenemy >= freeCells) {
        lastAction = "Ошибка: не хватает свободных клеток для врагов!";
        isLevelActive = false;
        return;
    }

    do {
        playerX = std::rand() % width;
        playerY = std::rand() % height;
    } while (field.GetCell(playerX, playerY).IsBlocked());

    for(int i = 0; i < countenemy; i++) {
        int enemyX, enemyY;
        do {
            enemyX = std::rand() % width;
            enemyY = std::rand() % height;
        } while(field.GetCell(enemyX, enemyY).IsBlocked() || 
                (enemyX == playerX && enemyY == playerY));
        
        enemies.push_back(Enemy());
        enemyPositions.push_back({enemyX, enemyY});
    }
    
    lastAction = "Уровень сгенерирован. Врагов: " + std::to_string(countenemy);
}

bool Game::NextLevel() {
    currentLevel++;
    lastAction = "Переход на уровень " + std::to_string(currentLevel);
    
    int newWidth = field.GetWidth() + 1;
    int newHeight = field.GetHeight() + 1;
    chance = std::min(1.0, chance + 0.1);
    baseEnemyCount += 3;

    player.RestoreHealth();
    hand.RemoveHalfSpells();

    GenerateLevel(newWidth, newHeight, chance, baseEnemyCount);

    return true; // чтобы потом менб апа пказать 
}

// тут всен игнроковое 

bool Game::CanMoveTo(int x, int y) const {
    if (x < 0 || y < 0 || x >= field.GetWidth() || y >= field.GetHeight())
        return false;
    
    if (field.GetCell(x, y).IsBlocked())
        return false;
    
    for (const auto& pos : enemyPositions) {
        if (pos.first == x && pos.second == y)
            return false;
    }
    
    return true;
}

bool Game::MovePlayerDirection(int dx, int dy) {
    if (dx == 0 && dy == 0) return false;
    
    int newX = playerX;
    int newY = playerY;
    int speed = player.GetSpeed();
    bool moved = false;
    
    for (int i = 0; i < speed; ++i) {
        int nextX = newX + dx;
        int nextY = newY + dy;
        
        if (!CanMoveTo(nextX, nextY))
            break;
            
        newX = nextX;
        newY = nextY;
        moved = true;
    }
    
    if (moved) {
        playerX = newX;
        playerY = newY;
        lastAction = "Игрок переместился на (" + std::to_string(playerX) + ", " + std::to_string(playerY) + ")";
        return true;
    } else {
        lastAction = "Невозможно перейти на эти координаты";
        return false;
    }
}

AttackResult Game::PerformAttackNearest() {
    AttackResult result = FightManager::Fight(player, enemies, enemyPositions, playerX, playerY);
    
    if (result.enemyDefeated) {
        UpdateAfterKill();
        lastAction = "Ты победила врага! Убийств: " + std::to_string(player.GetMurder());
    } else if (result.playerDefeated) {
        lastAction = "Ты проиграла в бою";
    } else {
        lastAction = result.message;
    }
    
    return result;
}

SpellResult Game::UseSpellByIndex(int index) {
    SpellResult result;
    
    if (index < 1 || index > static_cast<int>(hand.GetSpells().size())) {
        result.success = false;
        result.message = "Неверный индекс заклинания!";
        lastAction = result.message;
        return result;
    }
    
    result = hand.UseSpell(index, enemies, playerX, playerY, enemyPositions, -1, -1);
    lastAction = result.message;
    return result;
}

SpellResult Game::UseSpellByIndexWithCoords(int index, int targetX, int targetY) {
    SpellResult result;
    
    if (index < 1 || index > static_cast<int>(hand.GetSpells().size())) {
        result.success = false;
        result.message = "Неверный индекс заклинания!";
        lastAction = result.message;
        return result;
    }

    result = hand.UseSpell(index, enemies, playerX, playerY, enemyPositions, targetX, targetY);
    lastAction = result.message;
    return result;
}

bool Game::SwitchPlayerAttackType() {
    player.SwitchAttackType();
    lastAction = "Тип атаки изменен на: " + std::string(player.GetAttackType() == AttackType::MELEE ? "ближний бой" : "дальний бой");
    return true;
}

std::vector<EnemyInfo> Game::SearchForEnemies() {
    return FightManager::SearchEnemy(playerX, playerY, enemyPositions, enemies);
}

// все враговое 

std::vector<std::string> Game::EnemyTurn() {
    std::vector<std::string> enemyActions;
    
    if (justLoaded) {  
        justLoaded = false;
        return enemyActions;
    }
    
    MoveEnemies();
    
    auto attacks = EnemiesAttackIfPlayerNear();
    enemyActions.insert(enemyActions.end(), attacks.begin(), attacks.end());
    
    return enemyActions;
}

void Game::MoveEnemies() {
    Move::MoveEnemy(enemies, enemyPositions, field, playerX, playerY);
}

std::vector<std::string> Game::EnemiesAttackIfPlayerNear() {
    return FightManager::EnemiesAttackIfPlayerNear(player, enemies, enemyPositions, playerX, playerY);
}



bool Game::UpdateAfterKill() {
    if (player.GetMurder() > 0 && player.GetMurder() % 3 == 0 && lastSpellCount != player.GetMurder()) {
        bool added = hand.AddRandomSpell();
        if (added) {
            lastSpellCount = player.GetMurder();
            lastAction = "Добавлено новое заклинание!";
            return true;
        }
    }
    return false;
}

bool Game::ApplyUpgrade(int choice) {
    switch (choice) {
        case 1:
            player.UpdateMaxHealth(20);
            lastAction = "Максимальное здоровье увеличено на 20";
            return true;
        case 2:
            player.UpdateMeleeDamage(5);
            lastAction = "Ближний урон увеличен на 5";
            return true;
        case 3:
            player.UpdateRangedDamage(5);
            lastAction = "Дальний урон увеличен на 5";
            return true;
        case 4:
            player.UpdateSpeed(1);
            lastAction = "Скорость увеличена на 1";
            return true;
        default:
            lastAction = "Неверный выбор улучшения";
            return false;
    }
}

GameState Game::SaveToState() const {
    GameState state;
    state.SetPlayer(player);
    state.SetPlayerPosition(playerX, playerY);
    state.SetEnemies(enemies);
    state.SetEnemyPositions(enemyPositions);
    state.SetHand(hand);
    state.SetCurrentLevel(currentLevel);
    return state;
}

void Game::RestoreFromState(const GameState& state) {
    player = state.GetPlayer();
    playerX = state.GetPlayerX();
    playerY = state.GetPlayerY();
    enemies = state.GetEnemies();
    enemyPositions = state.GetEnemyPositions();
    hand = state.GetHand();
    currentLevel = state.GetCurrentLevel();
    justLoaded = true;
    lastAction = "Игра загружена";
}

bool Game::SaveGameToFile(const std::string& filename) {
    GameState state = SaveToState();
    bool success = StoreGame::SaveGame(state, filename);
    return success;
}

bool Game::LoadGameFromFile(const std::string& filename) {
    GameState state;
    if (StoreGame::LoadGame(state, filename)) {
        RestoreFromState(state);
        return true;
    } else {
        return false;
    }
}

bool Game::HasSpellRequiringCoords(int index) const {
    if (index < 1 || index > static_cast<int>(hand.GetSpells().size())) {
        return false;
    }
    
    const auto& spells = hand.GetSpells();
    return spells[index - 1]->GetName() == "Заклинание урона по площади";
}