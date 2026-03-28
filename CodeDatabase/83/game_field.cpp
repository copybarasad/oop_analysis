#include "game_field.h"
#include "player.h"
#include "enemy.h"
#include "book_of_spells.h"
#include "game_defines.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <stdexcept>

class LoadGameException : public std::runtime_error {
public:
    LoadGameException(const std::string& message) : std::runtime_error(message) {}
};

void GameField::initializeField() {
    field.resize(size, std::vector<Cell>(size));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
                field[i][j].setPassable(false);
            } else {
                field[i][j].setPassable(true);
            }
        }
    }

    std::srand(static_cast<unsigned int>(std::time(nullptr) + size));
    
    int rand_int = std::rand() % (size - 2) + 1;
    int start_j = std::rand() % (size - 2) + 1;
    for (int j = start_j; j < size - 1; j++) {
        field[j][rand_int].setPassable(false);
    }

    for (int j = 0; j < 5 + (size / 3); j++) {
        int x = std::rand() % (size - 2) + 1;
        int y = std::rand() % (size - 2) + 1;
        if (!(x == START_X && y == START_Y)) {
            field[y][x].setPassable(false);
        }
    }
}

GameField::GameField(int size, BookOfSpells& book) : size(size) {
    initializeField();
    createSpells(book);
    createTowers();
}

void GameField::saveState(std::ostream& file) const {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            file << (field[y][x].canPass() ? '1' : '0');
        }
        file << "\n";
    }
    
    file << spellsOnField.size() << "\n";
    for (const auto& spell : spellsOnField) {
        std::string spellName = spell.getSpellName();
        std::replace(spellName.begin(), spellName.end(), ' ', '_');
        file << spell.getX() << " " << spell.getY() << " " 
             << spellName << " " << spell.isCollected() << "\n";
    }
    
    file << traps.size() << "\n";
    for (const auto& trap : traps) {
        std::string spellName = trap.getSpellName();
        std::replace(spellName.begin(), spellName.end(), ' ', '_');
        file << trap.getX() << " " << trap.getY() << " "
             << spellName << " " << trap.getDamage() << " " 
             << trap.isActive() << "\n";
    }
    
    file << towers.size() << "\n";
    for (const auto& tower : towers) {
        file << tower.getX() << " " << tower.getY() << "\n";
    }
}

void GameField::loadState(std::istream& file, BookOfSpells& book) {
    std::string line;
        for (int y = 0; y < size; y++) {
        if (!std::getline(file, line)) {
            throw LoadGameException("Unexpected end of file while reading field data");
        }
        
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        
        if (line.length() != size) {
            throw LoadGameException("Invalid field data length in save file");
        }
        
        for (int x = 0; x < size; x++) {
            field[y][x].setPassable(line[x] == '1');
        }
    }
    
    int spellCount;
    if (!(file >> spellCount)) {
        throw LoadGameException("Cannot read spell count");
    }
    
    spellsOnField.clear();
    for (int i = 0; i < spellCount; i++) {
        int x, y, collected;
        std::string spellName;
        if (!(file >> x >> y >> spellName >> collected)) {
            throw LoadGameException("Cannot read spell data");
        }
        std::replace(spellName.begin(), spellName.end(), '_', ' ');
        FieldSpell spell(x, y, spellName);
        if (collected) spell.collect();
        spellsOnField.push_back(spell);
    }
    
    int trapCount;
    if (!(file >> trapCount)) {
        throw LoadGameException("Cannot read trap count");
    }
    
    traps.clear();
    for (int i = 0; i < trapCount; i++) {
        int x, y, damage, active;
        std::string spellName;
        if (!(file >> x >> y >> spellName >> damage >> active)) {
            throw LoadGameException("Cannot read trap data");
        }
        std::replace(spellName.begin(), spellName.end(), '_', ' ');
        Trap trap(x, y, spellName, damage);
        if (!active) trap.trigger();
        traps.push_back(trap);
    }
    
    int towerCount;
    if (!(file >> towerCount)) {
        throw LoadGameException("Cannot read tower count");
    }
    
    towers.clear();
    for (int i = 0; i < towerCount; i++) {
        int x, y;
        if (!(file >> x >> y)) {
            throw LoadGameException("Cannot read tower data");
        }
        towers.emplace_back(x, y, TOWER_ATTACK_RANGE, TOWER_ATTACK_COOLDOWN);
    }
    
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool GameField::canMoveTo(const Player& player, const std::vector<Enemy>& enemies, int x, int y) const {   
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }
    
    if (!field[y][x].canPass()) {
        return false;
    }
    
    if (hasTowerAt(x, y)) {
        return false;
    }
    
    if ((x == player.getX()) && (y == player.getY())){
        return false;
    }

    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
            return false;
        }
    }

    return true;
}

void GameField::display(const Player& player, const std::vector<Enemy>& enemies) const {
    std::cout << "\n=== GAME FIELD ===\n";

    std::cout << "   ";
    for (int x = 0; x < size; x++) {
        if (x < 10) std::cout << " ";
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cout << "   ";
    for (int x = 0; x < size; x++) {
        std::cout << "---";
    }
    std::cout << "\n";
    
    for (int y = 0; y < size; y++) {
        if (y < 10) std::cout << " ";
        std::cout << y << "|";
        
        for (int x = 0; x < size; x++) {
            if (player.getX() == x && player.getY() == y) {
                std::cout << " H "; 
            } else {
                bool enemyHere = false;
                for (const auto& enemy : enemies) {
                    if (enemy.isAlive() && enemy.getX() == x && enemy.getY() == y) {
                        std::cout << " E "; 
                        enemyHere = true;
                        break;
                    }
                }
                
                if (!enemyHere) {
                    bool towerHere = hasTowerAt(x, y);
                    if (towerHere) {
                        std::cout << " T "; 
                    } else {
                        bool trapHere = hasTrapAt(x, y);
                        if (trapHere) {
                            std::cout << " X "; 
                        } else {
                            bool spellHere = false;
                            for (const auto& spell : spellsOnField) {
                                if (!spell.isCollected() && spell.getX() == x && spell.getY() == y) {
                                    std::cout << " S "; 
                                    spellHere = true;
                                    break;
                                }
                            }
                            
                            if (!spellHere) {
                                std::cout << (field[y][x].canPass() ? " . " : " # ");
                            }
                        }
                    }
                }
            }
        }
        
        std::cout << "|" << y << "\n";
    }
    
    std::cout << "   ";
    for (int x = 0; x < size; x++) {
        std::cout << "---";
    }
    std::cout << "\n";
    
    std::cout << "   ";
    for (int x = 0; x < size; x++) {
        if (x < 10) std::cout << " ";
        std::cout << x << " ";
    }
    std::cout << "\n";
    
    std::cout << "Legend: H - Hero, E - Enemy, T - Tower, # - Wall, . - Empty, S - Spell, X - Trap\n";
}

bool GameField::inField(int x, int y) const {
    return x >= 0 && x < size && y >= 0 && y < size;
}

void GameField::createSpells(BookOfSpells& book) { 
    spellsOnField.clear();
    for (int i = 0; i < 3 + (size / 5); i++) { 
        int spellX, spellY;
        int attempts = 0;
        
        do {
            spellX = std::rand() % size;
            spellY = std::rand() % size;
            attempts++;
        } while ((!field[spellY][spellX].canPass() || 
                 hasTowerAt(spellX, spellY) ||
                 (spellX == START_X && spellY == START_Y)) && 
                 attempts < 50); 
        
        if (attempts < 50) {
            auto randomSpell = book.getRandomSpell();
            if (randomSpell) {
                spellsOnField.emplace_back(spellX, spellY, randomSpell->getName());
            }
        }
    }
}

void GameField::createTowers() {
    towers.clear();
    int numTowers = NUM_TOWERS + (size / 8); 
    
    for (int i = 0; i < numTowers; i++) {
        int towerX, towerY;
        int attempts = 0;
        
        do {
            towerX = std::rand() % size;
            towerY = std::rand() % size;
            attempts++;
        } while ((!field[towerY][towerX].canPass() || 
                 hasTowerAt(towerX, towerY) || 
                 (towerX == START_X && towerY == START_Y)) && 
                 attempts < 50);
        
        if (attempts < 50) {
            towers.emplace_back(towerX, towerY, TOWER_ATTACK_RANGE, TOWER_ATTACK_COOLDOWN);
        }
    }
}

FieldSpell* GameField::getSpellAtPosition(int x, int y) {
    for (auto& spell : spellsOnField) {
        if (!spell.isCollected() && spell.getX() == x && spell.getY() == y) {
            return &spell;
        }
    }
    return nullptr;
}

bool GameField::collectSpell(int x, int y) {
    FieldSpell* spell = getSpellAtPosition(x, y);
    if (spell) {
        spell->collect();
        return true;
    }
    return false;
}

bool GameField::hasSpells() const {
    for (const auto& spell : spellsOnField) {
        if (!spell.isCollected()) {
            return true;
        }
    }
    return false;
}

void GameField::respawnSpells(BookOfSpells& book) {
    if (!hasSpells()) {
        createSpells(book);
    }
}

void GameField::addTrap(const Trap& trap) {
    traps.push_back(trap);
}

void GameField::checkTraps(std::vector<Enemy>& enemies) {
    bool trapTriggered = false;
    
    for (auto& trap : traps) {
        if (trap.isActive()) {
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && trap.checkTrigger(enemy.getX(), enemy.getY(), enemy)) {
                    trapTriggered = true;
                    break;
                }
            }
        }
    }
    
    traps.erase(
        std::remove_if(traps.begin(), traps.end(), 
                      [](const Trap& t) { return !t.isActive(); }),
        traps.end()
    );
}

bool GameField::hasTrapAt(int x, int y) const {
    for (const auto& trap : traps) {
        if (trap.isActive() && trap.getX() == x && trap.getY() == y) {
            return true;
        }
    }
    return false;
}

void GameField::addTower(const Tower& tower) {
    towers.emplace_back(tower.getX(), tower.getY(), tower.getAttackRange(), TOWER_ATTACK_COOLDOWN);
}

void GameField::updateTowers(Player& player, std::vector<Enemy>& enemies) {
    for (auto& tower : towers) {
        tower.update(player, enemies);
    }
}

bool GameField::hasTowerAt(int x, int y) const {
    for (const auto& tower : towers) {
        if (tower.getX() == x && tower.getY() == y) {
            return true;
        }
    }
    return false;
}