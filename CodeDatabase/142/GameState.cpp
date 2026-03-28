#include "GameState.h"
#include <fstream>
#include <sstream>

namespace {
constexpr const char* HEADER = "GAME_SAVE_V1";
}

GameState::GameState() {
    clear();
}

void GameState::setPlayer(int x, int y, int hp, int maxHp, int score, int meleeDmg, int rangedDmg, int mode) {
    playerX = x;
    playerY = y;
    playerHP = hp;
    playerMaxHP = maxHp;
    playerScore = score;
    meleeDamage = meleeDmg;
    rangedDamage = rangedDmg;
    combatMode = mode;
}

void GameState::setHandMaxSize(int size) {
    handMaxSize = size;
}

void GameState::addSpell(const SpellData& spell) {
    spells.push_back(spell);
}

void GameState::setFieldSize(int width, int height) {
    fieldWidth = width;
    fieldHeight = height;
}

void GameState::addWall(int x, int y) {
    walls.emplace_back(x, y);
}

void GameState::addEnemy(int x, int y, int hp, int damage) {
    enemies.emplace_back(x, y);
    enemyHP.push_back(hp);
    enemyDamage.push_back(damage);
}

void GameState::setProgress(int level, int initialEnemies, int killed) {
    currentLevel = level;
    initialEnemyCount = initialEnemies;
    enemiesKilled = killed;
}

int GameState::getPlayerX() const { return playerX; }
int GameState::getPlayerY() const { return playerY; }
int GameState::getPlayerHP() const { return playerHP; }
int GameState::getPlayerMaxHP() const { return playerMaxHP; }
int GameState::getPlayerScore() const { return playerScore; }
int GameState::getMeleeDamage() const { return meleeDamage; }
int GameState::getRangedDamage() const { return rangedDamage; }
int GameState::getCombatMode() const { return combatMode; }
int GameState::getHandMaxSize() const { return handMaxSize; }
const std::vector<GameState::SpellData>& GameState::getSpells() const { return spells; }
int GameState::getFieldWidth() const { return fieldWidth; }
int GameState::getFieldHeight() const { return fieldHeight; }
const std::vector<std::pair<int, int>>& GameState::getWalls() const { return walls; }
const std::vector<std::pair<int, int>>& GameState::getEnemies() const { return enemies; }
const std::vector<int>& GameState::getEnemyHP() const { return enemyHP; }
const std::vector<int>& GameState::getEnemyDamage() const { return enemyDamage; }
int GameState::getCurrentLevel() const { return currentLevel; }
int GameState::getInitialEnemyCount() const { return initialEnemyCount; }
int GameState::getEnemiesKilled() const { return enemiesKilled; }

bool GameState::saveToFile(const std::string& filename, std::string& error) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        error = "Не удалось открыть файл для записи: " + filename;
        return false;
    }

    file << HEADER << "\n";

    file << "PLAYER\n";
    file << playerX << " " << playerY << " " << playerHP << " " << playerMaxHP << " "
         << playerScore << " " << meleeDamage << " " << rangedDamage << " " << combatMode << "\n";

    file << "HAND\n";
    file << handMaxSize << " " << spells.size() << "\n";
    for (const auto& spell : spells) {
        file << spell.type << " " << spell.radius << " " << spell.damage << "\n";
    }

    file << "FIELD\n";
    file << fieldWidth << " " << fieldHeight << "\n";
    file << walls.size() << "\n";
    for (const auto& wall : walls) {
        file << wall.first << " " << wall.second << "\n";
    }

    file << "ENEMIES\n";
    file << enemies.size() << "\n";
    for (size_t i = 0; i < enemies.size(); ++i) {
        file << enemies[i].first << " " << enemies[i].second << " "
             << enemyHP[i] << " " << enemyDamage[i] << "\n";
    }

    file << "GAME\n";
    file << currentLevel << " " << initialEnemyCount << " " << enemiesKilled << "\n";
    file << "END\n";

    if (!file || file.fail()) {
        error = "Ошибка записи в файл: " + filename;
        return false;
    }

    return true;
}

std::optional<GameState> GameState::loadFromFile(const std::string& filename, std::string& error) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        error = "Не удалось открыть файл для чтения: " + filename;
        return std::nullopt;
    }

    GameState state;

    std::string line;
    std::getline(file, line);
    if (line != HEADER) {
        error = "Неверный формат файла сохранения";
        return std::nullopt;
    }

    // PLAYER
    std::getline(file, line);
    if (line != "PLAYER") { error = "Ожидался раздел PLAYER"; return std::nullopt; }
    int px, py, php, pmax, pscore, pmd, prd, mode;
    if (!(file >> px >> py >> php >> pmax >> pscore >> pmd >> prd >> mode)) {
        error = "Ошибка чтения PLAYER";
        return std::nullopt;
    }
    file.ignore();
    state.setPlayer(px, py, php, pmax, pscore, pmd, prd, mode);

    // HAND
    std::getline(file, line);
    if (line != "HAND") { error = "Ожидался раздел HAND"; return std::nullopt; }
    int handSize, spellCount;
    if (!(file >> handSize >> spellCount)) { error = "Ошибка чтения HAND"; return std::nullopt; }
    if (spellCount < 0 || handSize <= 0) { error = "Некорректные параметры HAND"; return std::nullopt; }
    file.ignore();
    state.setHandMaxSize(handSize);
    for (int i = 0; i < spellCount; ++i) {
        GameState::SpellData s;
        if (!(file >> s.type >> s.radius >> s.damage)) {
            error = "Ошибка чтения заклинаний";
            return std::nullopt;
        }
        file.ignore();
        state.addSpell(s);
    }

    // FIELD
    std::getline(file, line);
    if (line != "FIELD") { error = "Ожидался раздел FIELD"; return std::nullopt; }
    int w, h;
    if (!(file >> w >> h)) { error = "Ошибка чтения FIELD"; return std::nullopt; }
    if (w <= 0 || h <= 0) { error = "Некорректные размеры поля"; return std::nullopt; }
    state.setFieldSize(w, h);
    file.ignore();
    int wallCount;
    if (!(file >> wallCount)) { error = "Ошибка чтения WALLS"; return std::nullopt; }
    if (wallCount < 0) { error = "Некорректное число стен"; return std::nullopt; }
    file.ignore();
    for (int i = 0; i < wallCount; ++i) {
        int wx, wy;
        if (!(file >> wx >> wy)) { error = "Ошибка чтения стен"; return std::nullopt; }
        file.ignore();
        state.addWall(wx, wy);
    }

    // ENEMIES
    std::getline(file, line);
    if (line != "ENEMIES") { error = "Ожидался раздел ENEMIES"; return std::nullopt; }
    int enemyCount;
    if (!(file >> enemyCount)) { error = "Ошибка чтения ENEMIES"; return std::nullopt; }
    if (enemyCount < 0) { error = "Некорректное число врагов"; return std::nullopt; }
    file.ignore();
    for (int i = 0; i < enemyCount; ++i) {
        int ex, ey, ehp, edmg;
        if (!(file >> ex >> ey >> ehp >> edmg)) { error = "Ошибка чтения врагов"; return std::nullopt; }
        file.ignore();
        state.addEnemy(ex, ey, ehp, edmg);
    }

    // GAME
    std::getline(file, line);
    if (line != "GAME") { error = "Ожидался раздел GAME"; return std::nullopt; }
    int lvl, initEnemies, killed;
    if (!(file >> lvl >> initEnemies >> killed)) { error = "Ошибка чтения GAME"; return std::nullopt; }
    state.setProgress(lvl, initEnemies, killed);
    file.ignore();

    std::getline(file, line);
    if (line != "END") { error = "Файл сохранения поврежден"; return std::nullopt; }

    if (!state.validate(error)) {
        return std::nullopt;
    }

    return state;
}

bool GameState::validate(std::string& error) const {
    if (fieldWidth <= 0 || fieldHeight <= 0) { error = "Размеры поля некорректны"; return false; }
    if (handMaxSize <= 0) { error = "Размер руки некорректен"; return false; }
    if (spells.size() > static_cast<size_t>(handMaxSize)) { error = "Слишком много заклинаний"; return false; }
    if (enemies.size() != enemyHP.size() || enemies.size() != enemyDamage.size()) {
        error = "Несогласованные данные врагов"; return false; }
    return true;
}

void GameState::clear() {
    playerX = playerY = 0;
    playerHP = playerMaxHP = 100;
    playerScore = 0;
    meleeDamage = 15;
    rangedDamage = 5;
    combatMode = 0;
    handMaxSize = 5;
    spells.clear();
    fieldWidth = fieldHeight = 10;
    walls.clear();
    enemies.clear();
    enemyHP.clear();
    enemyDamage.clear();
    currentLevel = 1;
    initialEnemyCount = 0;
    enemiesKilled = 0;
}

