#include "Game.h"
#include "GameRunner.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cctype>
#include <limits>
#include <fstream>


Game::Game()
    : player(1, "Hero", 100, CombatStyle::MELEE, 0, 0),
      enemies({
        Enemy(2, "Monster", 50, CombatStyle::LR, 9, 9),
        Enemy(3, "Monster", 80, CombatStyle::MELEE, 8, 9),
        Enemy(4, "Monster", 50, CombatStyle::LR, 7, 8),
      }),
      field(10, 10, {{1,5},{5,8},{9, 3}})
{
    if (!field.canMoveTo(player.getPosX(), player.getPosY())) {
        throw std::logic_error("Player spawn point is wall or occupied");
    }
    field.placeCharacter(player.getId(), player.getPosX(), player.getPosY(), true, false);
    
    for (auto& enemy : enemies) {
        if (!field.canMoveTo(enemy.getPosX(), enemy.getPosY())) {
            throw std::logic_error("Enemy spawn point is wall or occupied");
        }
        field.placeCharacter(enemy.getId(), enemy.getPosX(), enemy.getPosY(), false, true);
    }
}


void Game::printMap() const {
    //system("cls");
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << "          CURRENT GAME STATE\n";
    std::cout << std::string(40, '=') << "\n";

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            const auto& cell = field.getCell(x, y);

            if (cell.isPlayer) {
                std::cout << '@';
            } else if (cell.isEnemy) {
                std::cout << 'E';
            } else if (!field.isPassable(x, y)) {
                std::cout << '#';
            } else {
                std::cout << '.';
            }
        }
        std::cout << '\n';
    }

    std::cout << "\n--- Game STate ---\n";
    std::cout << "Player: " << player.getName() << " at (" << player.getPosX() << ", " << player.getPosY() << ")\n";
    std::cout << "HP: " << player.getHP() << " / " << player.getMaxHP() << "\n"
    << "Score: " << player.getScore() << "\n";
    for (auto& enemy : enemies) {
        std::cout << "Enemy: " << enemy.getName() 
        << " at (" << enemy.getPosX() << ", " << enemy.getPosY() << ") "
        << "HP: " << enemy.getHP() << "/" << enemy.getMaxHP() << "\n";
    }
    std::cout << "------------------\n";

    std::cout << "\n";
    std::cout << std::string(40, '=') << "\n";

    std::cout << "\nYour turn:\n";
    std::cout << "WASD - move, F - attack, M - visit spell shop, G - switch style, C - Save game, Q - quit\n";
    
    const auto& hand = player.getHand();
    if (hand.size() > 0) {
        std::cout << "Spells in hand:\n";
        for (size_t i = 0; i < hand.size(); ++i) {
            const Spell* spell = hand.getSpell(i);
            std::cout << "  [" << (i + 1) << "] " << spell->getName()
                      << " (cost: " << spell->getCost() << ")\n";
        }
        std::cout << "Press H to cast a spell\n";
    }
    std::cout << "Action: ";
}


void Game::setGameRunnerRef(GameRunner* runner) {
     gameRunnerRef = runner;
 }

 void Game::requestSave() {
     if (gameRunnerRef) {
         gameRunnerRef->saveGame();
     } else {
         std::cout << "Cannot save: GameRunner reference not set.\n";
         std::this_thread::sleep_for(std::chrono::milliseconds(800));
     }
}

void Game::getPlayerInput(){

    char input;
    std::cin >> input;

    if (std::toupper(input) == 'H'){
        castSpell();
        return;
    }
    
    if (std::toupper(input) == 'M') {
        visitShop();
        return;
    }

    if (std::toupper(input) == 'C'){
        requestSave();
        return;
    }

    switch (std::toupper(input)) {
        
        case 'G': {
            player.switchStyle();
            std::cout << "\nStyle changed\n";
            break;
        }
        
        case 'F': {
            bool attacked = false;
            for (auto& enemy:enemies){
                if (!enemy.getisAlive()) continue;
                int dx = std::abs(player.getPosX() - enemy.getPosX());
                int dy = std::abs(player.getPosY() - enemy.getPosY());
                
                bool inRange = (dx + dy <= 1) || (player.getCombatStyle() == CombatStyle::LR && dx <= 2 && dy <= dx + dy > 0);
                
                if (inRange) {
                    player.attack(enemy);
                    attacked = true;
                    if (!enemy.getisAlive()){
                        player.addScore(50);
                        field.removeCharacter(enemy.getPosX(), enemy.getPosY());
                    }
                    break;
                }
            }
            if(!attacked){
                std::cout << "No enemy in range!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
            break;
        }
        case 'W': {
            int newX = player.getPosX();
            int newY = player.getPosY() - 1;
            if (field.canMoveTo(newX, newY)) {
                field.moveCharacter(
                    player.getPosX(), player.getPosY(),
                    newX, newY,
                    player.getId(), true, false);
                player.setPos(newX, newY);
            } else {
                std::cout << "Cannot move there!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
            break;
        }
        case 'S':{
            int newX = player.getPosX();
            int newY = player.getPosY() + 1;
            if (field.canMoveTo(newX, newY)) {
                field.moveCharacter(
                    player.getPosX(), player.getPosY(),
                    newX, newY,
                    player.getId(), true, false
                );
                player.setPos(newX, newY);
            } else {
                std::cout << "Cannot move there!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
            break;
        }
        case 'A': {
            int newX = player.getPosX() - 1;
            int newY = player.getPosY();
            if (field.canMoveTo(newX, newY)) {
                field.moveCharacter(
                    player.getPosX(), player.getPosY(),
                    newX, newY,
                    player.getId(), true, false
                );
                player.setPos(newX, newY);
            } else {
                std::cout << "Cannot move there!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
            break;
        }
        case 'D': {
            int newX = player.getPosX() + 1;
            int newY = player.getPosY();
            if (field.canMoveTo(newX, newY)) {
                field.moveCharacter(
                    player.getPosX(), player.getPosY(),
                    newX, newY,
                    player.getId(), true, false
                );
                player.setPos(newX, newY);
            } else {
                std::cout << "Cannot move there!\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
            break;
        }
        case 'Q':{
            std::cout << "Goodbye!\n";
            exit(0);
        }

        default:{
            std::cout << "Unknown command!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(800));
            break;
        }
    }
}

void Game::castSpell(){
    auto& hand = player.getHand();
    if (hand.size() == 0){
        std::cout << "No spells!\n";
        return;
    }

    std::cout << "Choosr spell:\n";
    for (size_t i = 0; i < hand.size(); ++i){
        std::cout << "[" << (i+1) << "] " << hand.getSpell(i) -> getName() << "\n";
    }

    int idx;
    std::cin >> idx;
    if (idx <= 0 || idx > static_cast<int>(hand.size())){
        std::cout << "Invalid spell number!\n";
        return;
    }

    const Spell* spell = hand.getSpell(idx - 1);
    if (player.getScore() < spell->getCost()){
        std::cout << "Not enought score! You need " << spell->getCost() << ".\n";
        return;
    }

    player.addScore(-spell->getCost());

    if (spell->isAreaSpell()){
        int x, y;
        std::cout << "Enter EXPLOSION center (x,y): ";
        if (!(std::cin >> x >> y)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! use two numbers (e.g., '3 4').\n";
            player.addScore(spell->getCost());
            return;
        }

        if (x < 0 || x >=field.getWidth() || y < 0 || y >= field.getHeight()){
            std::cout << "Coordinates out of bounds! Field is " << field.getWidth() << "x" << field.getHeight() << ".\n";
            player.addScore(spell->getCost());
            return;
        }

        Spell* nonConstSpell = const_cast<Spell*>(spell);
        nonConstSpell->applyArea(player, x, y, *this);
    }else{
        bool foundTarget = false;
        for (auto& enemy : enemies) {
            if (!enemy.getisAlive()) continue;

            int dx = std::abs(player.getPosX() - enemy.getPosX());
            int dy = std::abs(player.getPosY() - enemy.getPosY());

           int spellRange = dynamic_cast<const Fireball*>(spell)->getRange();
            if (dx <= spellRange && dy <= spellRange) {
                Spell* nonConstSpell = const_cast<Spell*>(spell);
                nonConstSpell->apply(player, enemy);
                foundTarget = true;
                break;
            }
        }
    }
}


void Game::handleEnemyTurn() {
    if (!player.getisAlive()) return;

    for (auto& enemy : enemies){
        if (!enemy.getisAlive()) continue;
        
        int px = player.getPosX();
        int py = player.getPosY();
        int ex = enemy.getPosX();
        int ey = enemy.getPosY();

        int dx = px - ex;
        int dy = py - ey;

        if (std::abs(dx) + std::abs(dy) <= 1){
            enemy.attack(player);
        }else{
            int moveX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
            int moveY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);

            if (field.canMoveTo(ex + moveX, ey)) {
                field.moveCharacter(ex, ey, ex + moveX, ey, enemy.getId(), false, true);
                enemy.setPos(ex + moveX, ey);
            }else if (field.canMoveTo(ex, ey + moveY)){
                field.moveCharacter(ex, ey, ex, ey + moveY, enemy.getId(), false, true);
                enemy.setPos(ex, ey + moveY);
            }
        }

        if(!enemy.getisAlive()){
            player.addScore(100);
        }
    }
}

void Game::damageEnemyAt(int x, int y, int damage, int radius){
    for (auto& enemy : enemies){
        if (!enemy.getisAlive()) return;
        
        int ex = enemy.getPosX();
        int ey = enemy.getPosY();

        bool hit = false;
        if (radius == 0) {
            hit = (ex == x && ey == y);
        } else {
            hit = (std::abs(ex - x) <= radius && std::abs(ey - y) <= radius);
        }

        if (hit){
            enemy.takeDamage(damage);
            if(!enemy.getisAlive()){
                player.addScore(100);
                field.removeCharacter(ex, ey);
            }
        }
    }
}


void Game::showSpellMenu() {
    std::cout << "\n=== SPELL SHOP ===\n";
    for (size_t i = 0; i < spellShop.size(); ++i) {
        const Spell* spell = spellShop.getSpell(i);
        if (player.getHand().hasSpell(spell->getId())) {
            std::cout << "[" << (i + 1) << "] " << spell->getName() << " (owned)\n";
        } else {
            std::cout << "[" << (i + 1) << "] " << spell->getName()
                      << " - " << spell->getCost() << " points\n";
        }
    }
    std::cout << "[0] Exit shop\n";
    std::cout << "Your score: " << player.getScore() << "\n";
}

void Game::visitShop() {
    std::cout << "Entering spell shop...\n";
    while (true) {
        showSpellMenu();
        std::cout << "Choose spell to buy (0 to exit): ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            std::cout << "Leaving shop.\n";
            break;
        }

        buySpell(choice);
        // Небольшая пауза для чтения сообщений
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
}

void Game::buySpell(int spellIndex) {
    // Проверка диапазона
    if (spellIndex <= 0 || spellIndex > static_cast<int>(spellShop.size())) {
        std::cout << "Invalid spell number!\n";
        return;
    }

    const Spell* spell = spellShop.getSpell(spellIndex - 1);
    if (player.getHand().hasSpell(spell->getId())) {
        std::cout << "You already have " << spell->getName() << "!\n";
        return;
    }

    if (player.getScore() < spell->getCost()) {
        std::cout << "Not enough score! Need " << spell->getCost() << " points.\n";
        return;
    }

    // Создаём новое заклинание (клонируем)
    std::unique_ptr<Spell> newSpell;
    if (spell->isAreaSpell()) {
        // Предполагается, что BOMB принимает (id, name, cost, damage, radius)
        newSpell = std::make_unique<BOMB>(
            spell->getId(), spell->getName(), spell->getCost(), 50, 2
        );
    } else {
        newSpell = std::make_unique<Fireball>(
            spell->getId(), spell->getName(), spell->getCost(), 5, 1
        );
    }

    if (player.getHand().addSpell(std::move(newSpell))) {
        player.addScore(-spell->getCost());
        std::cout << "Purchased " << spell->getName() << "!\n";
    } else {
        std::cout << "Cannot add spell: hand is full or spell already owned.\n";
    }
}

bool Game::allEnemiesDead() const {
    for (const auto& enemy : enemies) {
        if (enemy.getisAlive()) return false;
    }
    return true;
}

void Game::run() {
    while (player.getisAlive() && !allEnemiesDead()) {
        printMap();
        getPlayerInput();
        if (allEnemiesDead()) break;
        handleEnemyTurn();
    }

    printMap();
    if (player.getisAlive() && allEnemiesDead()) {
        std::cout << "You win!\n";
    } else {
        std::cout << "You lose!\n";
    }
}


void Game::save(std::ostream& out) const{
    player.save(out);
    size_t numEnemies = enemies.size();
    out.write(reinterpret_cast<const char*>(&numEnemies), sizeof(numEnemies));
    for (const auto& enemy : enemies) {
        enemy.save(out);
    }
    field.save(out);
}


void Game::load(std::istream& in){
    player.load(in);
    size_t numEnemies;
    in.read(reinterpret_cast<char*>(&numEnemies), sizeof(numEnemies));
    if (numEnemies > 10) {
        throw std::runtime_error("Suspicious number of enemies read from save file: " + std::to_string(numEnemies));
    }

    enemies.clear();
    enemies.reserve(numEnemies);
    for (size_t i = 0; i < numEnemies; ++i) {
        Enemy enemy(0, "tmp", 1, CombatStyle::MELEE, 0, 0);
        enemy.load(in);
        enemies.push_back(std::move(enemy));
    }
    field.load(in);

}

bool Game::isPlayerAlive() const {
    return player.getisAlive();
}
