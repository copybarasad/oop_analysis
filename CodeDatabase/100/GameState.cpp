#include "GameState.h"

// Конструктор
GameState::GameState(int fieldWidth, int fieldHeight, int playerHealth, int playerDamage):
                    field(fieldWidth, fieldHeight), player(playerHealth, playerDamage, 1, 1), 
                    hand(3), turnsSinceLastSpell(0), gameRunning(true), level(1) { }

// Деструктор
GameState::~GameState() { 
    clearGameState();
}

// Конструктор копирования 
GameState::GameState(const GameState& other)
    : field(other.field),
      player(other.player),
      hand(other.hand),
      characters(),                    
      buildings(),                       
      messageLog(other.messageLog),
      towers(),                        
      allies(),                        
      gameRunning(other.gameRunning),
      lastTrapDamage(other.lastTrapDamage),
      level(other.level),
      turnsSinceLastSpell(other.turnsSinceLastSpell) {
    
    copyFrom(other);
}

// Move конструктор 
GameState::GameState(GameState&& other) noexcept
    : field(std::move(other.field)),
      player(std::move(other.player)),
      hand(std::move(other.hand)),
      characters(std::move(other.characters)),       
      buildings(std::move(other.buildings)),          
      messageLog(std::move(other.messageLog)),
      towers(std::move(other.towers)),                
      allies(std::move(other.allies)),               
      gameRunning(other.gameRunning),
      lastTrapDamage(other.lastTrapDamage),
      level(other.level),
      turnsSinceLastSpell(other.turnsSinceLastSpell) {
    
    for (auto& character : characters) {
        if (character == &other.player) {
            character = &player;
            break;
        }
    }
    
    other.characters.clear();
    other.buildings.clear();
    other.towers.clear();
    other.allies.clear();
}

// Оператор присваивания копированием 
GameState& GameState::operator=(const GameState& other) {
    if (this != &other) {
        clearGameState();

        field = other.field;
        player = other.player;
        hand = other.hand;
        messageLog = other.messageLog;
        gameRunning = other.gameRunning;
        lastTrapDamage = other.lastTrapDamage;
        level = other.level;
        turnsSinceLastSpell = other.turnsSinceLastSpell;
        
        copyFrom(other);
    }
    return *this;
}

// Move оператор присваивания
GameState& GameState::operator=(GameState&& other) noexcept {
    if (this != &other) {
        clearGameState();
        
        field = std::move(other.field);
        player = std::move(other.player);
        hand = std::move(other.hand);
        messageLog = std::move(other.messageLog);
        gameRunning = other.gameRunning;
        lastTrapDamage = other.lastTrapDamage;
        level = other.level;
        turnsSinceLastSpell = other.turnsSinceLastSpell;
        
        characters = std::move(other.characters);
        buildings = std::move(other.buildings);
        towers = std::move(other.towers);
        allies = std::move(other.allies);
        
        for (auto& character : characters) {
            if (character == &other.player) {
                character = &player;
                break;
            }
        }
        
        other.characters.clear();
        other.buildings.clear();
        other.towers.clear();
        other.allies.clear();
    }
    return *this;
}

// Вспомогательный метод для глубокого копирования
void GameState::copyFrom(const GameState& other) {
    // Копируем characters (кроме игрока)
    for (const auto& character : other.characters) {
        if (character != &other.player) {
            // Создаем копию врага
            characters.push_back(new Enemy(*dynamic_cast<Enemy*>(character)));
        } else {
            // Указываем на нашего игрока (который уже скопирован)
            characters.push_back(&player);
        }
    }
    
    // Копируем buildings
    for (const auto& building : other.buildings) {
        buildings.push_back(new EnemyBuilding(*building));
    }
    
    // Копируем towers
    for (const auto& tower : other.towers) {
        towers.push_back(new EnemyTower(*tower));
    }
    
    // Копируем allies
    for (const auto& ally : other.allies) {
        allies.push_back(new Ally(*ally));
    }
}

void GameState::clearGameState() {
    for (auto& character : characters) {
        if (character != &player) delete character;
    }
    characters.clear();
    for (auto& building : buildings) delete building;
    buildings.clear();
    for (auto& tower : towers) delete tower;
    towers.clear();
    for (auto& ally : allies) delete ally;
    allies.clear();
    messageLog.clear();
    hand.clearHand();
}

void GameState::clearHand() {
    hand.clearHand();
}

// Геттеры
Player& GameState::getPlayer() { return player; }
const GameField& GameState::getField() const { return field; }
const PlayerHand& GameState::getHand() const { return hand; }
const std::vector<Character*>& GameState::getAllCharacters() const { return characters; }
const std::vector<EnemyBuilding*>& GameState::getAllBuildings() const { return buildings; }
const std::vector<std::string>& GameState::getMessageLog() const { return messageLog; }
const std::vector<EnemyTower*>& GameState::getAllTowers() const { return towers; }
const std::vector<Ally*>& GameState::getAllAllies() const { return allies; }
bool GameState::isGameRunning() const { return gameRunning; }

void GameState::setGameRunning(bool running) { gameRunning = running; }

// Добавить персонажа / здание / башню / союзника
void GameState::addTower(EnemyTower* tower) { 
    if (tower) towers.push_back(tower); 
}

void GameState::addCharacter(Character* character) {
    if (!character) return;

    // Если это первый персонаж, например Player, не добавляем дубликаты
    for (auto c : characters) {
        if (c == character)
            return;
    }

    characters.push_back(character);
}

void GameState::addAlly(Ally* ally) {
    if (ally) allies.push_back(ally);
}

void GameState::addBuilding(EnemyBuilding* building) {
    if (building) buildings.push_back(building);
}

bool GameState::hasAliveEnemies() const {
    for (const auto& character : characters) {
        if (character != &player && character->isAlive()) {
            return true;
        }
    }
    for (const auto& building : buildings) {
        if (building->isAlive()) {
            return true;
        }
    }
    for (const auto& tower : towers) {
        if (tower->isAlive()) {
            return true;
        }
    }
    return false;
}

// Работа с логами
void GameState::addMessage(const std::string& message) {
    messageLog.push_back(message);
}


void GameState::clearMessageLog() {
    messageLog.clear();
}

// Установить ловушку в клетке
void GameState::setCellTrap(int x, int y, bool hasTrap) {
    if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
        field.setTrap(x, y, hasTrap);
    }
}

// Установить / Получить урон ловушки
void GameState::setLastTrapDamage(int damage) { lastTrapDamage = damage; }
int GameState::getLastTrapDamage() const { return lastTrapDamage; }

void GameState::incrementTurnsSinceLastSpell() { turnsSinceLastSpell++; }
void GameState::resetTurnsSinceLastSpell() { turnsSinceLastSpell = 0; }
bool GameState::shouldGiveSpell() const { return turnsSinceLastSpell >= SPELL_DROP_INTERVAL && !hand.isFull(); }
bool GameState::addSpellToHand(std::shared_ptr<Spell> spell) { return hand.addSpell(spell); }
bool GameState::useSpellFromHand(size_t spellIndex, int targetX, int targetY) { 
    return hand.useSpell(spellIndex, player, *this, targetX, targetY);
}
bool GameState::upgradeSpellInHand(size_t spellIndex) {
    return hand.upgradeSpell(spellIndex);
}

// Метод для удаления заклинания из руки
bool GameState::removeSpellFromHand(size_t spellIndex) {
    return hand.removeSpell(spellIndex);
}

int GameState::getLevel() const {
    return level;
}

void GameState::setLevel(int lvl) {
    level = lvl;
}