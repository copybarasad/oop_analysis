#include "GameEngine.h"
#include "SaveManager.h"

GameEngine::GameEngine(int fieldWidth, int fieldHeight)
    : field(fieldWidth, fieldHeight), player(100, 50, 8, 100, 3), gameActive(true) {
    field.setPlayerDamage(player.getCurrentDamage());
}

bool GameEngine::processPlayerMovement(int deltaX, int deltaY) {
     if (!player.isAlive()) {
        gameActive = false;
    }
    if (!gameActive) {
        return false;
    }
    
    Position currentPos = field.getPlayerPosition();
    Position targetPos(currentPos.getX() + deltaX, currentPos.getY() + deltaY);
    
    auto enemyPositions = field.getEnemyPositions();
    for (size_t i = 0; i < enemyPositions.size(); ++i) {
        if (enemyPositions[i] == targetPos && field.isEnemyAlive(i)) {
            damageEnemy(i, player.getCurrentDamage());
            return true;
        }
    }
    
    bool moved = field.movePlayer(deltaX, deltaY);
    return moved;
}

void GameEngine::switchPlayerCombatMode() {
    if (!gameActive) {
        return;
    }
    
    player.switchCombatMode();
    field.setPlayerDamage(player.getCurrentDamage());
}

int GameEngine::processEnemyTurns() {
    if (!gameActive) {
        return 0;
    }
    
    int healthBefore = player.getHealth();
    field.moveEnemies(player);
    int damageTaken = healthBefore - player.getHealth();
    
    if (!player.isAlive()) {
        gameActive = false;
    }
    
    return damageTaken;
}

void GameEngine::addEnemy(std::unique_ptr<Enemy> enemy, const Position& position) {
    field.addEnemy(std::move(enemy), position);
}

bool GameEngine::castSpell(int spellIndex, const Position& targetPosition) {
    if (!gameActive || !player.isAlive()) {
        return false;
    }
    
    bool spellUsed = player.castSpell(spellIndex, targetPosition, *this);
    
    if (spellUsed) {
        processEnemyTurns();
    }
    
    return spellUsed;
}

bool GameEngine::canBuySpell() const {
    return player.canBuySpell();
}

void GameEngine::buyRandomSpell() {
    player.buyRandomSpell();
}

bool GameEngine::isGameActive() const {
    return gameActive && player.isAlive();
}

int GameEngine::getPlayerHealth() const {
    return player.getHealth();
}

int GameEngine::getPlayerScore() const {
    return player.getScore();
}

CombatMode GameEngine::getPlayerCombatMode() const {
    return player.getCombatMode();
}

Position GameEngine::getPlayerPosition() const {
    return field.getPlayerPosition();
}

int GameEngine::getAliveEnemyCount() const {
    return field.getAliveEnemyCount();
}

bool GameEngine::setPlayerPosition(const Position& pos) {
    return field.setPlayerPosition(pos);
}


CellType GameEngine::getCellType(const Position& pos) const {
    return field.getCellType(pos);
}

std::vector<Position> GameEngine::getEnemyPositions() const {
    return field.getEnemyPositions();
}

std::vector<int> GameEngine::getEnemyHealths() const {
    return field.getEnemyHealths();
}

std::vector<int> GameEngine::getEnemyDamages() const {
    return field.getEnemyDamages();
}

void GameEngine::damageEnemy(int index, int damage) {
     if (index < 0 || index >= field.getEnemyPositions().size()) {
        return;
    }
    
    auto healthsBefore = field.getEnemyHealths();
    bool wasAliveBefore = (index < healthsBefore.size()) && (healthsBefore[index] > 0);
    
    field.damageEnemy(index, damage);
    
    auto healthsAfter = field.getEnemyHealths();
    bool isAliveAfter = (index < healthsAfter.size()) && (healthsAfter[index] > 0);
    
    if (wasAliveBefore && !isAliveAfter) {
        player.onEnemyKilled();
    }
}

int GameEngine::getPlayerDamage() const {
    return player.getCurrentDamage();
}


int GameEngine::getPlayerMana() const {
    return player.getMana();
}

int GameEngine::getPlayerMaxMana() const {
    return player.getMaxMana();
}

const SpellHand& GameEngine::getPlayerSpellHand() const {
    return player.getSpellHand();
}



std::string GameEngine::getSpellName(int index) const {
    return player.getSpellHand().getSpellName(index);
}

int GameEngine::getSpellManaCost(int index) const {
    return player.getSpellHand().getSpellManaCost(index);
}

int GameEngine::getWidth() const {
    return field.getWidth();
}

int GameEngine::getHeight() const {
    return field.getHeight();
}
bool GameEngine::isValidPosition(const Position& pos) const {
    return field.isValidPosition(pos);
}

bool GameEngine::isEnemyAlive(int index) const {
    return field.isEnemyAlive(index);
}


bool GameEngine::isEnemyTower(int index) const {
    return field.isEnemyTower(index);
}

int GameEngine::getEnemyAttackRange(int index) const {
    return field.getEnemyAttackRange(index);
}


void GameEngine::setPlayerHealth(int health) {
    player.setHealth(health);
}

void GameEngine::setPlayerScore(int score) {
    player.setScore(score);
}

void GameEngine::setPlayerMana(int mana) {
    player.setMana(mana);
}

void GameEngine::setPlayerCombatMode(CombatMode mode) {
    player.setCombatMode(mode);
}

std::vector<Position> GameEngine::getTrapPositions() const {
    return field.getTrapPositions();
}


void GameEngine::damageBuilding(const Position& pos, int damage) {
    field.damageBuilding(pos, damage);
}


bool GameEngine::isEnemyBuildingAt(const Position& pos) const {
    return field.isEnemyBuildingAt(pos);
}
void GameEngine::saveGameState(std::ofstream& file) const {
    SaveManager saveManager;
    saveManager.saveGameState(file, this);
}

void GameEngine::loadGameState(std::ifstream& file) {
    SaveManager saveManager;
    saveManager.loadGameState(file, this);
}
bool GameEngine::placeTrap(const Position& position, int damage) {
    return field.placeTrap(position, damage);
}