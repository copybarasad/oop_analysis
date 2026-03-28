#include "GameControl.h"
#include "../spells/DamageSpellContext.h"
#include "../spells/SpawnSpellContext.h"
#include "../spells/UpgradeSpellContext.h"
#include "../spells/EnemySpellContext.h"
#include "../spells/SpellManager.h"

const std::vector<std::pair<int, int>> GameControl::towerDirections = {
        {0, 1}, {-1, -1}, {0, -1}, {1, -1},
        {-1, 0}, {1, 0},{-1, 1}, {1, 1}
    };

const std::vector<std::pair<int,int>> GameControl::enemiesDirections = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},
    };

GameControl::GameControl(int width, int height, int enemies, int slowingCells, int wallsCount, int fortressesCount, int lvl)
    : world(std::make_unique<GameWorld>(width, height))
    , spawner(enemies, slowingCells, wallsCount, fortressesCount, lvl){
        spawner.spawnStartObjects(*world);
}

GameControl::~GameControl() = default;

void GameControl::movePlayer(int dx, int dy, bool& tookTurn) {
    auto& player = world->getPlayer();
    auto& field = world->getField();
    if (player.isSlowed()){
        player.removeSlow();
        return;
    }
    int newX = player.getX() + dx;
    int newY = player.getY() + dy;
    
    if (!(field.isValidMove(newX, newY)) || (field.isValidMove(newX, newY) && (field.getCell(newX,newY).getType() == Cell::CellType::ALLY || field.getCell(newX,newY).getType() == Cell::CellType::TRAP))) {
        std::cout << "Невозможно переместиться в эту клетку!" << std::endl;
        tookTurn = false;
        return;
    }

    playerAttack(newX,newY,player.getDamage());
    bool cellIsEmptyNow = (field.getCell(newX,newY).getType() == Cell::CellType::EMPTY);
    if (cellIsEmptyNow){
        world->moveCharacter(player.getX(), player.getY(),newX, newY,player);
        if (field.getCell(newX, newY).isSlowing()) {
            player.getSlowedDown();
        }
    }
    tookTurn = true;
}

bool GameControl::playerAttack(int targX, int targY, int damage, std::string prompt){
    auto& field = world->getField();
    auto& player = world->getPlayer();
    bool enemyFound = false;
    if (field.getCell(targX,targY).getType() == Cell::CellType::BUILDING){
        enemyFound = true;
        Building& tower = world->getTower();
        tower.takeDamage(damage);
        std::cout << prompt << " вражескую башню! Нанесено " << damage << " урона." << std::endl;
            if (!tower.isAlive()){
                std::cout << "Вражеская башня уничтожена!" << std::endl;
            }
        return enemyFound;
    }

    auto& fortresses = world->getFortresses();
    for (auto it = fortresses.begin(); it != fortresses.end(); ) {
        if ((*it)->getX() == targX && (*it)->getY() == targY) {
            enemyFound = true;
            (*it)->takeDamage(damage);
            std::cout << prompt << " вражескую крепость! Нанесено " << damage << " урона." << std::endl;
            player.updateCoins(3);
            removeDeadEneties();
            return enemyFound;
        } else {
            ++it;
        }
    }

    auto& enemies = world->getEnemies();
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if ((*it)->getX() == targX && (*it)->getY() == targY) {
            enemyFound = true;
            (*it)->takeDamage(damage);
            std::cout << prompt << " врага! Нанесено " << damage << " урона." << std::endl;
            player.updateCoins(3);
            removeDeadEneties();
            return enemyFound;
        } else {
            ++it;
        }
    }
    return enemyFound;
}


bool GameControl::villainAttack(int targX, int targY, int damage, std::string prompt){
    auto& player = world->getPlayer();
    auto& field = world->getField();
    bool heroFound = false;
    if (field.getCell(targX,targY).getType() == Cell::CellType::PLAYER){
        heroFound = true;
        player.takeDamage(damage);
        std::cout << prompt << " вас! Потеряно " << damage << " здоровья!" << std::endl;
        return heroFound;
    }
    auto& allies = world->getAllies();
    for (auto it = allies.begin(); it != allies.end(); ) {
        if ((*it)->getX() == targX && (*it)->getY() == targY) {
            heroFound = true;
            (*it)->takeDamage(damage);
            std::cout << prompt << " союзника! Нанесено " << damage << " урона." << std::endl;
            return heroFound;
        } else {
            ++it;
        }
    }
    return heroFound;
}

void GameControl::castSpell(int index, bool& tookTurn, std::pair<int, int> target){
    auto& player = world->getPlayer();
    switch (index)
    {
    case 0: case 1:{
        auto damageContext = DamageSpellContext(*world, target);
        player.getPlayerHand().getSpell(index)->cast(damageContext);
        printSpellResults(damageContext.getResults());
        break;
    }
    case 2: case 3:{
        auto spawnContext = SpawnSpellContext(*world, spawner, target);
        player.getPlayerHand().getSpell(index)->cast(spawnContext);
        break;
    }
    default:
        break;
    }
    tookTurn = player.getPlayerHand().getSpell(index)->wasUsed();
    if(player.getPlayerHand().getSpell(index)->wasUsed()){
        player.getPlayerHand().removeSpell(index);
    }
    else{
        std::cout << "Невозможно применить данное заклинание!" << std::endl;
    }
    removeDeadEneties();
}

void GameControl::castUpgradeSpell(int index, bool& tookTurn, int UpgradeInd){
    auto& player = world->getPlayer();
    auto upgradeContext = UpgradeSpellContext(player.getPlayerHand().getSpell(UpgradeInd));
    if(index < 0 || index > 4 || player.getPlayerHand().getSpellNum(UpgradeInd) == 0){
        std::cout<< "Невозможно применить данное заклинание!" << std::endl;
        return;
    }
    player.getPlayerHand().getSpell(index)->cast(upgradeContext);
    tookTurn = player.getPlayerHand().getSpell(index)->wasUsed();
    player.getPlayerHand().removeSpell(4);
}

void GameControl::longRangeAttack(int dx, int dy){
    auto& player = world->getPlayer();
    auto& field = world->getField();
    for(int i = 1; i < 4; i++){
        int targetX = player.getX() + dx*i;
        int targetY = player.getY() + dy*i;
        if(field.isValidMove(targetX,targetY)){
            if(playerAttack(targetX,targetY,player.getDamage())){
                return;
            }
        }
    }
}

void GameControl::removeDeadEneties(){
    auto& player = world->getPlayer();
    auto& field = world->getField();
    auto& enemies = world->getEnemies();
    auto& fortresses = world->getFortresses();
    auto& allies = world->getAllies();
    for(auto it = enemies.begin(); it != enemies.end(); ){
        if (!(*it)->isAlive()){
            std::cout << "Враг убит! + 10 очков." << std::endl;
            player.addScore(10);
            player.updateCoins(6);
            field.getCell((*it)->getX(), (*it)->getY()).setType(Cell::CellType::EMPTY);
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
    for(auto it = fortresses.begin(); it != fortresses.end(); ){
        if (!(*it)->isAlive()){
            std::cout << "Крепость уничтожена! + 15 очков." << std::endl;
            player.addScore(15);
            player.updateCoins(10);
            field.getCell((*it)->getX(), (*it)->getY()).setType(Cell::CellType::EMPTY);
            it = fortresses.erase(it);
        } else {
            ++it;
        }
    }
    for(auto it = allies.begin(); it != allies.end(); ){
        if (!(*it)->isAlive()){
            std::cout << "Союзник убит!" << std::endl;
            field.getCell((*it)->getX(), (*it)->getY()).setType(Cell::CellType::EMPTY);
            it = allies.erase(it);
        } else {
            ++it;
        }
    }
}

void GameControl::moveAlly(){
    auto& allies = world->getAllies();
    auto& field = world->getField();
    auto& player = world->getPlayer();
    for (auto& ally : allies) {
        if (!ally->isAlive()) continue;
        int allyX = ally->getX();
        int allyY = ally->getY();
        bool allyMoved = false;

        for (auto& otherCell : enemiesDirections) {
            int newX = allyX + otherCell.first;
            int newY = allyY + otherCell.second;
            if(field.isValidMove(newX, newY) and (field.isStructure(newX,newY) || field.getCell(newX,newY).getType()==Cell::CellType::ENEMY)){
                allyMoved = true;
                playerAttack(newX,newY,ally->getDamage(),"Союзник атаковал");
                removeDeadEneties();
                if (field.getCell(newX,newY).getType()==Cell::CellType::EMPTY){
                    world->moveCharacter(allyX, allyY, newX, newY, *ally);
                }
                break;
            }
        }
        if(!allyMoved){
            int playerX = player.getX();
            int playerY = player.getY();
            int dx = 0, dy = 0;

            if (std::abs(playerX - allyX) > std::abs(playerY - allyY)) {
                dx = (playerX > allyX) ? 1 : -1;
            } else {
                dy = (playerY > allyY) ? 1 : -1;
            }

            int newX = allyX + dx;
            int newY = allyY + dy;

            if(field.isValidMove(newX, newY) && field.getCell(newX,newY).getType()==Cell::CellType::EMPTY){
                world->moveCharacter(allyX, allyY, newX, newY, *ally);
            }
        }
    }
    removeDeadEneties();
}

bool GameControl::gameContinues(){
    auto& player = world->getPlayer();
    Building& tower = world->getTower();
    return player.isAlive() && tower.isAlive();
}

void GameControl::moveEnemies() {
    auto& player = world->getPlayer();
    auto& enemies = world->getEnemies();
    auto& field = world->getField();
    
    for (auto& enemy : enemies) {
        if (!enemy->isAlive()) continue;
        
        int enemyX = enemy->getX();
        int enemyY = enemy->getY();
        bool enemyMadeMove = false;

        for (auto& dir : enemiesDirections) {
            int attackX = enemyX + dir.first;
            int attackY = enemyY + dir.second;
            
            if (field.isValidMove(attackX, attackY)) {
                if (villainAttack(attackX, attackY, enemy->getDamage())) {
                    enemyMadeMove = true;
                    break;
                }
            }
        }

        if (!enemyMadeMove) {
            int playerX = player.getX();
            int playerY = player.getY();
            int dx = 0, dy = 0;

            if (std::abs(playerX - enemyX) > std::abs(playerY - enemyY)) {
                dx = (playerX > enemyX) ? 1 : -1;
            } else {
                dy = (playerY > enemyY) ? 1 : -1;
            }

            int newX = enemyX + dx;
            int newY = enemyY + dy;

            if (field.isValidMove(newX, newY) && (field.getCell(newX, newY).getType() == Cell::CellType::EMPTY || field.getCell(newX, newY).getType() == Cell::CellType::TRAP)) {
                world->moveCharacter(enemyX, enemyY, newX, newY, *enemy);
                enemyMadeMove = true;
            } 
            else {
                for (auto& dir : enemiesDirections) {
                    newX = enemyX + dir.first;
                    newY = enemyY + dir.second;
                    if (field.isValidMove(newX, newY) && (field.getCell(newX, newY).getType() == Cell::CellType::EMPTY || field.getCell(newX, newY).getType() == Cell::CellType::TRAP)) {
                        world->moveCharacter(enemyX, enemyY, newX, newY, *enemy);
                        enemyMadeMove = true;
                        break;
                    }
                }
            }
        }
        auto& traps = world->getTraps();
        for (auto it = traps.begin(); it != traps.end(); ) {
            if ((*it)->getX() == enemy->getX() && (*it)->getY() == enemy->getY()) {
                enemy->takeDamage((*it)->getDamage());
                std::cout << "Враг попал в ловушку! Нанесено " << (*it)->getDamage() << " урона" << std::endl;
                it = traps.erase(it);
            } else {
                ++it;
            }
        }
    }
    removeDeadEneties();
}

void GameControl::towerSpawn(){
    Building& tower = world->getTower();
    auto& field = world->getField();
    tower.updateCounter();
    
    if (tower.spawnIsReady()){
        for (auto& otherCell : towerDirections){
            int newX = tower.getX() + otherCell.first;
            int newY = tower.getY() + otherCell.second;
            if (field.isValidMove(newX,newY) && (field.getCell(newX,newY).getType() != Cell::CellType::ENEMY) && (field.getCell(newX,newY).getType() != Cell::CellType::PLAYER)){
                spawner.spawnEnemy(*world,newX,newY);
                return;
            }
        }
    }
}

void GameControl::FortressAttack(){
    auto& fortresses = world->getFortresses();
    for(auto& fortress : fortresses){
        fortress->updateCounter();
        if(fortress->attackIsReady()){
            auto FSpell = fortress->getFortressSpell();
            auto enemyContext = EnemySpellContext(*world, {fortress->getX(), fortress->getY()});
            FSpell.cast(enemyContext);
            printSpellResults(enemyContext.getResults());
        }
    }
    removeDeadEneties();
}

void GameControl::printSpellResults(damageSpellResults results){
    for(int i = 0; i < results.woundedAllies; i++){
        std::cout << "Вражеская крепость атаковала союзника!" << " Нанесено " << results.damage << " урона" << std::endl;
    }
    for(int i = 0; i < results.woundedPlayers; i++){
        std::cout << "Вас атаковала вражеская крепость!" << " Нанесено " << results.damage << " урона" << std::endl;
    }
    for(int i = 0; i < results.woundedFortresses; i++){
        std::cout << "Вы атаковали вражескую крепость заклинанием!" << " Нанесено" << results.damage << " урона" << std::endl;
    }
    for(int i = 0; i < results.woundedTowers; i++){
        std::cout << "Вы атаковали башню заклинанием!" << " Нанесено " << results.damage << " урона" << std::endl;
    }
    for(int i = 0; i < results.woundedEnemies; i++){
        std::cout << "Вы атаковали врага заклинанием!" << " Нанесено " << results.damage << " урона" << std::endl;
    }
}

GameWorld& GameControl::getWorld(){
    return *world;
}

const GameWorld& GameControl::getWorld() const{
    return *world;
}