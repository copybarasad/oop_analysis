#include "gamemanager.hpp"

void Game::advance(){
    std::cout << "\n";

    std::cout << "Choose a stat up!\n";
    std::cout << "VIT(1)  STR(2)  INT(3)\n";
    std::string input;
    std::getline(std::cin, input);

    if(input == "1")
        player->setMaxHealth(player->getMaxHealth() + 50);
    else if(input == "2")
        player->setDamage(player->getDamage() * 2);
    else if(input == "3")
        magichand.setSpellDamage(magichand.getSpellDamage() + 10);
    else{
        std::cout << "WRONG!!!\n";
        advance();
    }

}

bool Game::advanceToNextLevel() {
    if (level >= 4) {
        std::cout << "VICTORY...for real this time! You defeated everything!\n";
        std::cout << "Final Score: " << getPlayer()->getScore() << "\n";
        return false; 
    }
    
    std::cout << "VICTORY or is it...? You defeated all enemies but EVIL presence lingers...\n";
    std::cout << "Level Score: " << getPlayer()->getScore() << "\n";
    level++;
    std::cout << "LEVEL " << level << "\n";
    
    
    allCharacters.clear();
    
    
    int newWidth = 2 * (gameMap.getWidth() - 5);
    int newHeight = 2 * (gameMap.getHeight() - 5);
    gameMap = GameMap(newWidth, newHeight);
    
    
    movement.~Movement();
    new (&movement) Movement(gameMap);
    
    combat.~Combat();
    new (&combat) Combat(gameMap);
    
    enemyManager.~EnemyManager();
    new (&enemyManager) EnemyManager(gameMap, allCharacters);
    
    magichand.~MagicHand();
    new (&magichand) MagicHand(gameMap);
    magichand.setPlayer(player.get());
    magichand.setAllCharacters(&allCharacters);
    movement.setMagicHand(&magichand);
    
    advance();
    initializeGame();
    
    return true; 
}

Game::Game(std::string name)
    : gameMap(name),
      movement(gameMap),
      combat(gameMap),
      enemyManager(gameMap, allCharacters),
      magichand(gameMap){
    

    saveFile f(name);
        
    json j = json::parse(f.get());
    if(!j.contains("characters"))  throw FileFormatException("Missing required field 'characters'");
    if(!j.contains("spells")) throw FileFormatException("Missing required field 'spells'");
    if(!j.contains("game_state")) throw FileFormatException("Missing required field 'game_state'");
    if(!j.contains("map")) throw FileFormatException("Missing required field 'map'");
    loadGame(j);
    
}

Game::Game(int width, int height) 
    : gameMap(width, height),
      movement(gameMap),
      combat(gameMap),
      enemyManager(gameMap, allCharacters),
      magichand(gameMap){
    
    initializeGame();
    
}

void Game::initializeGame() {
    if(!player){
        player = std::make_unique<Player>(100, 20, 0, 0);
        magichand.setPlayer(player.get());
    } else{
        player->setHealth(player->getMaxHealth());
        player->setX(0);
        player->setY(0);
        
    }
    gameMap.placeCharacter(player.get(), 0, 0);
    magichand.setAllCharacters(&allCharacters);
    movement.setMagicHand(&magichand);
    
    enemyManager.spawnInitialEnemies(2*level);
    enemyManager.spawnTowers(level, level);
}

void Game::loadGame(json& j){
    if (!j.contains("player") || !j.contains("game_state")) {
        throw LoadException("Missing required sections in save file");
    }

    json& gameState = j["game_state"];
    json& playerData = j["player"];

    if (!gameState.contains("level")) {
        throw LoadException("Missing 'level' in game_state");
    }

    std::vector<std::string> requiredPlayerFields = {
        "health", "damage", "x", "y", "weapon", "slow_stat", 
        "score", "mx_health"
    };

    for (const auto& field : requiredPlayerFields) {
        if (!playerData.contains(field)) {
            throw LoadException("Missing player field: " + field);
        }
    }

    level = gameState["level"];
    if (level < 1) {
        throw GameException("Invalid level value: " + std::to_string(level));
    }

    int health = playerData["health"];
    int damage = playerData["damage"];
    int x = playerData["x"];
    int y = playerData["y"];
    int maxHealth = playerData["mx_health"];

    if (health <= 0 || maxHealth <= 0) {
        throw GameException("Invalid health values (health: " + 
                            std::to_string(health) + ", maxHealth: " + 
                            std::to_string(maxHealth) + ")");
    }

    if (health > maxHealth) {
        throw GameException("Current health exceeds max health");
    }

    if (damage < 0) {
        throw GameException("Negative damage value: " + std::to_string(damage));
    }

    if (x < 0 || y < 0) {
        throw GameException("Invalid player position (x,y)");
    }
     json tp = j["player"];
    level = j["game_state"]["level"];
    player = std::make_unique<Player>(tp["health"], tp["damage"], tp["x"], tp["y"]);
    player->setWeapon(tp["weapon"]);
    player->setSlowed(tp["slow_stat"]);
    player->setScore(tp["score"]);
    player->setDamage(tp["damage"]);
    player->setMaxHealth(tp["mx_health"]);
    magichand.setPlayer(player.get());
    magichand.setInventory(j["spells"]);

    gameMap.placeCharacter(player.get(), player->getX(), player->getY());
    magichand.setAllCharacters(&allCharacters);
    movement.setMagicHand(&magichand);

    enemyManager.LoadEnemies(j["characters"]);
}

void Game::processPlayerTurn(const std::string& command) {
    if (command == "w" || command == "a" || command == "s" || command == "d") {
        if (player->isSlowed()) {
            std::cout << "Player is slowed and cannot move this turn!\n";
            player->clearSlow();
            return;
        }
        handlePlayerMovement(command);
    } else if (command == "switch") {
        player->switchWeapons();
        std::cout << "Switched to " << (player->getWeapon() == RANGED ? "RANGED" : "MELEE") << " weapon\n";
    } else if(command.rfind("cast", 0) == 0){
        magichand.handleInput(command);
    }
}

void Game::processEnemyTurns() {
    if (!player) return;
    for (auto& character : allCharacters) {
        Ally* ally = dynamic_cast<Ally*>(character.get());
        if (!ally || !ally->isAlive()) continue;

        Enemy* nearest = nullptr;
        int bestDist = 1e9;
        for (auto& other : allCharacters) {
            Enemy* e = dynamic_cast<Enemy*>(other.get());
            if (!e || !e->isAlive()) continue;
            int dx = std::abs(e->getX() - ally->getX());
            int dy = std::abs(e->getY() - ally->getY());
            int dist = dx + dy;
            if (dist < bestDist) { bestDist = dist; nearest = e; }
        }
        if (!nearest) continue;

        if (bestDist == 1) {
            combat.performMeleeAttack(ally, nearest);
            continue;
        }

        std::string dir;
        int ax = ally->getX();
        int ay = ally->getY();
        int ex = nearest->getX();
        int ey = nearest->getY();
        if (std::abs(ex - ax) >= std::abs(ey - ay)) {
            dir = (ex > ax) ? "d" : "a";
        } else {
            dir = (ey > ay) ? "s" : "w";
        }
        movement.moveCharacter(ally, dir);
    }
    for (auto& character : allCharacters) {
        Enemy* enemy = dynamic_cast<Enemy*>(character.get());
        if (enemy && enemy->isAlive()) {
            int playerX = player->getX();
            int playerY = player->getY();
            int enemyX = enemy->getX();
            int enemyY = enemy->getY();
            
            int directions[4][2] = {
                {-1, 0},  
                {1, 0},   
                {0, -1},  
                {0, 1}
            };
            
            bool canAttack = false;
            for (int i = 0; i < 4; i++) {
                int checkX = enemyX + directions[i][0];
                int checkY = enemyY + directions[i][1];
                
                if (gameMap.getCharacterAt(checkX, checkY) == getPlayer()) {
                    combat.performMeleeAttack(enemy, getPlayer());
                    canAttack = true;
                    break;
                }
            }
            
            if (!canAttack) {
                std::string direction;
                if (std::rand() % 2 == 0 || playerY == enemyY) {
                    direction = playerX > enemyX ? "d" : "a";
                } else {
                    direction = playerY > enemyY ? "s" : "w";
                }
                
                movement.moveCharacter(enemy, direction);
            }
        }
    }

}

void Game::updateBurning(){
    for(auto& c: allCharacters){
        Character* character = c.get();
        if(!character) continue;
        if(character->isBurning()){
            character->updateBurning();
            std::cout << character->getIcon()<< " is burning! damage received 5\n";
            character->takeDamage(5); 
        }
    }
    if(getPlayer() && getPlayer()->isBurning()){
        getPlayer()->updateBurning();
        std::cout << getPlayer()->getIcon()<< " is burning! damage received 5\n";
        getPlayer()->takeDamage(5); 
    }
}



void Game::updateGameState() {
    enemyManager.updateTowers();
    enemyManager.updateMageTowers(player.get());
    updateBurning();
}

void Game::displayGame() const {
    gameMap.display();
    
    if (player) {
        std::cout << "\nPlayer - HP: " << player->getHealth() 
                  << "/" << player->getMaxHealth() 
                  << " | Damage: " << player->getDamage()
                  << " | Score: " << player->getScore() 
                  << " | Weapon: " << (player->getWeapon() == RANGED ? "RANGED" : "MELEE")
                  << (player->isSlowed() ? " | SLOWED" : "")
                  << (player->isBurning() ? " | BURNING" :"") << "\n";
    }
    
    int aliveEnemies = std::count_if(allCharacters.begin(), allCharacters.end(),
        [](const auto& ptr) { return dynamic_cast<Character*>(ptr.get()) && ptr->isAlive(); });
    
    std::cout << "Enemies alive: " << aliveEnemies << "\n";
    const_cast<MagicHand&>(magichand).displayHand();
    std::cout << "Type 'help' to see available commands\n\n";
}

bool Game::isGameOver() const {
    return !player || !player->isAlive();
}

bool Game::playerWon() const {
    for (const auto& character : allCharacters) {
        Character* enemy = dynamic_cast<Character*>(character.get());
        if(enemy->getIcon() == 'A') continue;
        if (enemy && enemy->isAlive()) {
            return false;
        }
    }
    return true; 
}

Player* Game::getPlayer() const {
    return player.get();
}

void Game::handlePlayerMovement(const std::string& direction) {
    int currentX = player->getX();
    int currentY = player->getY();
    
    int dx = 0, dy = 0;
    movement.getDirectionOffset(direction, dx, dy);
    Player* ptrPlayer = getPlayer();
    Character* rangedTarget = combat.findRangedTarget(
        ptrPlayer, currentX, currentY, dx, dy);
    
    if (rangedTarget) {
        combat.performRangedAttack(ptrPlayer, rangedTarget);
    } else if (movement.moveCharacter(ptrPlayer, direction)) {
        player->addScore(1);
    } else {
        int newX = currentX + dx;
        int newY = currentY + dy;
        Character* target = gameMap.getCharacterAt(newX, newY);
        if (target && (dynamic_cast<Enemy*>(target) || dynamic_cast<Tower*>(target))) {
            combat.performMeleeAttack(ptrPlayer, target);
            if (!target->isAlive()) {
                player->addScore(10);
            }
        }
    }
}

void Game::saveGameState(){
    json j;
    gameMap.saveMap(j);
    j["game_state"] = {
        {"width", gameMap.getWidth()},
        {"height",gameMap.getWidth()},
        {"level", level}
    };
    json magArray = json::array();
    j["spells"] = magichand.getInventory();
    j["player"] = {
        {"icon",     player->getIcon()},
        {"x",        player->getX()},
        {"y",        player->getY()},
        {"health",   player->getHealth()},
        {"mx_health",player->getMaxHealth()},
        {"damage",   player->getDamage()},
        {"score",    player->getScore()},
        {"burning",  player->isBurning()},
        {"burn_cooldown", player->getBurnCooldown()},
        {"weapon",   player->getWeapon()},
        {"slow_stat",player->isSlowed()},
        
    };

    json charArray = json::array();

    for (auto& c : allCharacters){
        charArray.push_back({
            {"icon",     c->getIcon()},
            {"x",        c->getX()},
            {"y",        c->getY()},
            {"health",   c->getHealth()},
            {"mx_health", c->getMaxHealth()},
            {"burning",  c->isBurning()},
            {"burn_cooldown", c->getBurnCooldown()},
            {"damage",   c->getDamage()},
        });
        Tower* tower = dynamic_cast<Tower*>(c.get());
        if(tower){
            charArray.back()["tot_cooldown"] = tower->getTotalCooldown();
            charArray.back()["cur_cooldown"] = tower->getCurrentCooldown();
        }
        MageTower* mtower = dynamic_cast<MageTower*>(c.get());
        if(mtower){
            charArray.back()["radius"] = mtower->getAttackRadius();

        }
        
    }

    j["characters"] = charArray;

    std::ofstream SaveFile("savefile.json");
    SaveFile << j.dump(4);
}
