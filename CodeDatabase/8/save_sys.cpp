#include "save_sys.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "enemytower.h"
#include "field.h"
#include "node.h"
#include "hand.h"
#include "spell.h"
#include "spellitems.h"
#include <iostream>
#include <cctype> 
#include <limits>
using namespace std;
bool SaveSystem::isNumber(const string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
Spell* SaveSystem::createSpellByName(const std::string& name) {
    if (name == "Fireball") return new Fireball();
    if (name == "Ice Bolt") return new IceBolt();
    if (name == "Fire Wave") return new FireWave();
    if (name == "Ice Storm") return new IceStorm();
    if (name == "Poison Trap") return new PoisonTrap();
    return nullptr;
}
void SaveSystem::saveGame(const Game& game) {
    ofstream file(saveFileName);
    
    if (!file.is_open()) {
        throw runtime_error("Не удалось создать файл сохранения!");
    }
    
    file << "УРОВЕНЬ: " << game.getCurrentLevel() << endl;
    
    saveField(file, *game.getField());
    savePlayer(file, *game.getPlayer());
    saveEnemy(file, *game.getEnemy());
    saveTower(file, *game.getTower());
    
    file.close();
    cout << "Игра полностью сохранена!" << endl;
}

void SaveSystem::saveField(ofstream& file, const Field& field) {
    file << "ПОЛЕ: " << field.getwidth() << " " << field.getlength() << endl;
    file << "КЛЕТКИ: ";
    
    for (int x = 0; x < field.getwidth(); x++) {
        for (int y = 0; y < field.getlength(); y++) {
            Type cellType = field.getNode(x, y).getType();
            file << static_cast<int>(cellType) << " ";
        }
    }
    file << endl;
}

void SaveSystem::savePlayer(ofstream& file, const Player& player) {
    file << "ИГРОК: " << player.getname() << " "
         << player.getX() << " " << player.getY() << " "
         << player.getLives() << " " << player.getPoints() << " "
         << static_cast<int>(player.getCombatType()) << " "
         << player.getPower() << " " << player.getRadius() << endl;
    
    const Hand& hand = player.getHand();
    file << "ЗАКЛИНАНИЯ: " << hand.getSpellCount() << " ";
    
    for (int i = 0; i < hand.getSpellCount(); i++) {
        Spell* spell = hand.getSpell(i);
        string spellName = spell->getName();
        for (char& c : spellName) {
            if (c == ' ') c = '_';
        }
        file << spellName << " ";
    }
    file << endl;
}

void SaveSystem::saveEnemy(ofstream& file, const Enemy& enemy) {
    file << "ВРАГ: " << enemy.getname() << " "
         << enemy.getX() << " " << enemy.getY() << " "
         << enemy.getLives() << " " << enemy.getDamage() << endl;
}

void SaveSystem::saveTower(ofstream& file, const EnemyTower& tower) {
    string towerName = tower.getName();
    for (char& c : towerName) {
        if (c == ' ') c = '_';
    }
    
    file << "БАШНЯ: " << towerName << " "
         << tower.getX() << " " << tower.getY() << " "
         << tower.getAttackRange() << " " << tower.canAttackNow() << endl;
}

bool SaveSystem::saveExists() const {
    ifstream file(saveFileName);
    return file.good();
}

void SaveSystem::deleteSave() {
    remove(saveFileName.c_str());
}

bool SaveSystem::loadGame(Game& game) {
    ifstream file(saveFileName);
    
    if (!file.is_open()) {
        throw runtime_error("Файл сохранения не найден!");
    }
    
    try {
        string token;
        cout << endl;
        file >> token;
        if (token != "УРОВЕНЬ:") {
            throw runtime_error("Некорректный формат файла сохранения");
        }
        
        int level;
        file >> level;
        cout << "УРОВЕНЬ " << level << endl;
        game.setCurrentLevel(level);
        loadField(file, game);
        loadPlayer(file, game);
        loadPlayerSpells(file, game);
        loadEnemy(file, game);
        loadTower(file, game);
        file.close();
        return true;
        
    } catch (const exception& e) {
        cout << "Ошибка загрузки: " << e.what() << endl;
        file.close();
        return false;
    }
}


void SaveSystem::loadField(ifstream& file, Game& game) {
    string token;
    file >> token;
    if (token != "ПОЛЕ:") {
        throw runtime_error("Ошибка формата: ожидалось 'ПОЛЕ:'");
    }
    
    int width, length;
    file >> width >> length;
    cout << "Загружаем поле размером: " << width << "x" << length << endl;
    game.getField()->resize(width, length);

    file >> token;
    if (token != "КЛЕТКИ:") {
        throw runtime_error("Ошибка формата: ожидалось 'КЛЕТКИ:'");
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < length; y++) {
            int cellType;
            file >> cellType;
            
            // отладочный вывод показать как читается
            if (x < 3 && y < 3) {
                cout << "Клетка [" << x << "," << y << "] = " << cellType << endl;
            }
            
            Node& node = game.getField()->getNode(x, y);
            switch (static_cast<Type>(cellType)) {
                case Type::EMPTY: 
                    node.setEmpty(); 
                    break;
                case Type::BUSY: 
                    node.setObstacle(); 
                    break;
                case Type::OBSTACLE: 
                    node.setObstacle(); 
                    break;
                case Type::TRAP: 
                    node.setTrap(4);
                    break;
                case Type::TOWER: 
                    node.setTower(); 
                    break;
                case Type::PLAYER:  
                case Type::ENEMY:
                    node.setEmpty(); 
                    break;
                default: 
                    node.setEmpty(); 
                    break;
            }
        }
    }
}

void SaveSystem::loadPlayer(ifstream& file, Game& game) {  
    string token;
    file >> token;
    
    if (token != "ИГРОК:") {
        throw runtime_error("Ошибка формата: ожидалось 'ИГРОК:'");
    }

    string name;
    file >> name;
    cout << "Загружаем игрока: '" << name << "'" << endl;
    
    int x, y, lives, points, combatType, power, radius;
    file >> x >> y >> lives >> points >> combatType >> power >> radius;

    if (file.fail()) {
        file.clear();
    }
    
    Player* player = game.getPlayer();
    player->setName(name);
    player->setPosition(x, y);
    if (game.getField()->validpos(x, y)) {
        game.getField()->getNode(x, y).setPlayer();
    }
    player->setLives(lives);
    player->setPoints(points);
    player->setCombatType(static_cast<Player::CombatType>(combatType));
}

void SaveSystem::loadPlayerSpells(ifstream& file, Game& game) {    
    string token;
    file >> token;
    
    if (token != "ЗАКЛИНАНИЯ:") {
        throw runtime_error("Ошибка формата: ожидалось 'ЗАКЛИНАНИЯ:'");
    }
    
    int spellCount;
    file >> spellCount;
    
    Player* player = game.getPlayer();
    Hand& hand = player->getHand();
    hand.clearHand();

    for (int i = 0; i < spellCount; i++) {
        string spellName;
        file >> spellName;       
        for (char& c : spellName) {
            if (c == '_') c = ' ';
        }
        Spell* spell = createSpellByName(spellName);
        if (spell) {
            hand.addSpell(spell);
            cout << "Добавлено: " << spellName << endl;
        } else {
            cout << "Неизвестное заклинание: " << spellName << endl;
        }
    }
}

void SaveSystem::loadEnemy(ifstream& file, Game& game) {
    string token;
    file >> token; // "ВРАГ:"
    if (token != "ВРАГ:") {
        throw runtime_error("Ошибка формата: ожидалось 'ВРАГ:'");
    }
    string name;
    file >> name;
    int x, y, lives, damage;
    file >> x >> y >> lives >> damage;
    
    Enemy* enemy = game.getEnemy();
    enemy->setName(name);
    enemy->setPosition(x, y);
    if (game.getField()->validpos(x, y)) {
        game.getField()->getNode(x, y).setEnemy();
    }
    enemy->setLives(lives);
    enemy->setDamage(damage);
    
    cout << "Враг загружен: " << name 
         << " позиция (" << x << "," << y << ")"
         << " жизни " << lives << " урон " << damage << endl;
}

void SaveSystem::loadTower(ifstream& file, Game& game) {
    string token;
    file >> token; // "БАШНЯ:"
    if (token != "БАШНЯ:") {
        throw runtime_error("Ошибка формата: ожидалось 'БАШНЯ:'");
    }
    
    string name;
    int x, y, range, canAttack;
    file >> name >> x >> y >> range >> canAttack;
    for (char& c : name) {
        if (c == '_') c = ' ';
    }
    EnemyTower* tower = game.getTower();
    tower->setName(name);
    tower->setPosition(x, y);
    if (game.getField()->validpos(x, y)) {
        game.getField()->getNode(x, y).setTower();
    }
    tower->setCanAttack(canAttack != 0);
    
    cout << "Башня загружена: " << name 
         << " позиция (" << x << "," << y << ")"
         << " может атаковать: " << (canAttack ? "да" : "нет") << endl;
}