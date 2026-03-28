#include "game_controller.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "game_constants.h"

#include <random>
#include <algorithm>
#include <fstream>
#include <cstdint>

GameController::GameController(int fieldWidth, int fieldHeight, int initialEnemyCount)
    : gameField(fieldWidth, fieldHeight),
    spellShop(this) {
    if (initialEnemyCount > 5) throw TooManyEnemiesException(initialEnemyCount);
    resetGame();
}

void GameController::processPlayerMove(MoveDirection direction) {
    if (!gameActive) return;
    Position newPos = calculateNewPosition(direction);
    if (gameField.isValidPosition(newPos)) {
        tryMovePlayer(newPos);
    }
}

bool GameController::castSpell(int spellIndex, const Position& target) {
    if (!gameActive || spellIndex < 0 || spellIndex >= player.getHand().getSize()) return false;

    SpellCard* spell = player.getHand().getSpell(spellIndex);
    if (!spell || spell->isUsed()) return false;

    if (auto* direct = dynamic_cast<DirectDamageSpell*>(spell)) {
        direct->setTarget(target);
    }
    else if (auto* area = dynamic_cast<AreaDamageSpell*>(spell)) {
        area->setTarget(target);
    }

    if (player.castSpell(spellIndex)) {
        if (spell->isUsed()) player.removeSpell(spellIndex);
        return true;
    }
    return false;
}

bool GameController::buySpell(int shopIndex) {
    if (!gameActive) return false;
    return spellShop.buySpell(player, shopIndex);
}

void GameController::updateGame() {
    if (!gameActive) return;
    moveEnemies();
    applyDamageFromNearbyEnemies();
    checkGameConditions();
    checkSpellRewards();
}

void GameController::moveEnemies() {
    Position playerPos = player.getPosition();
    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;
        Position enemyPos = enemy.getPosition();
        if (enemy.canAttackPlayer(playerPos)) continue;

        int dx = playerPos.getX() - enemyPos.getX();
        int dy = playerPos.getY() - enemyPos.getY();

        int moveX = 0, moveY = 0;
        if (std::abs(dx) > std::abs(dy)) {
            moveX = (dx > 0) ? 1 : -1;
        }
        else if (std::abs(dy) > 0) {
            moveY = (dy > 0) ? 1 : -1;
        }
        else if (std::abs(dx) > 0) {
            moveX = (dx > 0) ? 1 : -1;
        }

        Position newPos(enemyPos.getX() + moveX, enemyPos.getY() + moveY);
        if (gameField.isValidPosition(newPos) && gameField.isPositionEmpty(newPos)) {
            gameField.clearCell(enemyPos);
            enemy.setPosition(newPos);
            gameField.setCellType(newPos, CellType::ENEMY);
        }
    }
}

void GameController::applyDamageFromNearbyEnemies() {
    Position playerPos = player.getPosition();
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.canAttackPlayer(playerPos)) {
            player.takeDamage(15);
            break;
        }
    }
}

void GameController::checkGameConditions() {
    if (!player.isAlive()) {
        gameActive = false;
        return;
    }

    bool allDead = std::all_of(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return !e.isAlive(); });
    if (allDead && gameActive) {
        ++currentLevel;
        int bonus = 100 * currentLevel;
        player.addScore(bonus);

        enemies.clear();
        gameField = GameField(gameField.getWidth(), gameField.getHeight());
        enemiesKilled = 0;

        int newEnemyCount = 3 + (currentLevel - 1) * 2;
        initializeGame(newEnemyCount);
    }
}

void GameController::checkSpellRewards() {
    int currentKills = std::count_if(enemies.begin(), enemies.end(),
        [](const Enemy& e) { return !e.isAlive(); });
    if (currentKills > enemiesKilled && currentKills % 3 == 0 && player.canLearnSpell()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);
        if (dis(gen) == 0) {
            player.learnSpell(std::make_unique<DirectDamageSpell>(
                "Fire Bolt", "Deals 25 damage to single target within 3 cells", 25, 3, this));
        }
        else {
            player.learnSpell(std::make_unique<AreaDamageSpell>(
                "Fireball", "Deals 20 damage in 2x2 area within 4 cells", 20, 4, 2, this));
        }
    }
    enemiesKilled = currentKills;
}

Position GameController::calculateNewPosition(MoveDirection dir) const {
    Position cur = player.getPosition();
    int nx = cur.getX(), ny = cur.getY();
    switch (dir) {
    case MoveDirection::UP:    ny--; break;
    case MoveDirection::DOWN:  ny++; break;
    case MoveDirection::LEFT:  nx--; break;
    case MoveDirection::RIGHT: nx++; break;
    default: break;
    }
    return Position(nx, ny);
}

void GameController::initializeGame(int enemyCount) {
    placePlayerAtStart();
    placeEnemies(enemyCount);
    player.setScore(300);
}

void GameController::placePlayerAtStart() {
    Position start(0, 0);
    player.setPosition(start);
    gameField.setCellType(start, CellType::PLAYER);
}

void GameController::placeEnemies(int enemyCount) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distX(1, gameField.getWidth() - 1);
    std::uniform_int_distribution<int> distY(1, gameField.getHeight() - 1);

    for (int i = 0; i < enemyCount; ++i) {
        Position pos;
        bool placed = false;
        int attempts = 0;
        while (!placed && attempts < 100) {
            pos = Position(distX(gen), distY(gen));
            if (gameField.isPositionEmpty(pos)) placed = true;
            ++attempts;
        }
        if (placed) {
            enemies.emplace_back(pos);
            gameField.setCellType(pos, CellType::ENEMY);
        }
    }
}

bool GameController::tryMovePlayer(const Position& newPos) {
    Position old = player.getPosition();
    if (gameField.isPositionEmpty(newPos)) {
        gameField.clearCell(old);
        player.setPosition(newPos);
        gameField.setCellType(newPos, CellType::PLAYER);
        return true;
    }
    if (gameField.getCellType(newPos) == CellType::ENEMY) {
        for (auto& enemy : enemies) {
            if (enemy.getPosition() == newPos && enemy.isAlive()) {
                enemy.takeDamage(player.getDamage());
                if (!enemy.isAlive()) {
                    gameField.clearCell(newPos);
                    player.addScore(GameConstants::ENEMY_KILL_SCORE);
                }
                break;
            }
        }
    }
    return false;
}

void GameController::resetGame() {
    currentLevel = 1;
    player = Player();
    enemies.clear();
    gameField = GameField(gameField.getWidth(), gameField.getHeight());
    enemiesKilled = 0;
    gameActive = true;
    initializeGame(3);
}

void GameController::startNewGame() {
    resetGame();
}

bool GameController::saveGame(const std::string& filename) const {
    try {
        BinaryFile file(filename, std::ios::out);
        saveImpl(file.stream());
        return true;
    }
    catch (...) {
        return false;
    }
}

bool GameController::loadGame(const std::string& filename) {
    try {
        BinaryFile file(filename, std::ios::in);
        loadImpl(file.stream());
        return true;
    }
    catch (const FileOpenException&) {
        return false;
    }
    catch (...) {
        return false;
    }
}

void GameController::saveImpl(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&MAGIC), sizeof(MAGIC));
    out.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));
    player.save(out);
    uint32_t ec = static_cast<uint32_t>(enemies.size());
    out.write(reinterpret_cast<const char*>(&ec), sizeof(ec));
    for (const auto& e : enemies) e.save(out);
    gameField.save(out);
    out.write(reinterpret_cast<const char*>(&enemiesKilled), sizeof(enemiesKilled));
    out.write(reinterpret_cast<const char*>(&gameActive), sizeof(gameActive));
    out.write(reinterpret_cast<const char*>(&currentLevel), sizeof(currentLevel));
}

void GameController::loadImpl(std::istream& in) {
    uint32_t magic;
    in.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    if (magic != MAGIC)
        throw CorruptedDataException("save.bin", "invalid magic");

    uint8_t version;
    in.read(reinterpret_cast<char*>(&version), sizeof(version));
    if (version != VERSION)
        throw VersionMismatchException("save.bin", VERSION, version);

    player.load(in);

    uint32_t ec;
    in.read(reinterpret_cast<char*>(&ec), sizeof(ec));
    enemies.clear();
    enemies.reserve(ec);
    for (uint32_t i = 0; i < ec; ++i) {
        enemies.emplace_back(Position(0, 0));
        enemies.back().load(in);
    }

    gameField.load(in);

    in.read(reinterpret_cast<char*>(&enemiesKilled), sizeof(enemiesKilled));
    in.read(reinterpret_cast<char*>(&gameActive), sizeof(gameActive));
    in.read(reinterpret_cast<char*>(&currentLevel), sizeof(currentLevel));
}