#include "hero.h"
#include "../abstractNinterfaces/defines.h"
#include "../map.h"
#include "../spells/flameStorm.h"
#include "../spells/iceSpear.h"
#include <iostream>
#include <limits>
void hero::environmentInteraction(map* map, coords futureMapPosition)
{
    char tileName = map->getTiles()[futureMapPosition.y][futureMapPosition.x].name;
    switch(tileName){
        case TRAP:
        case NOTHING:
            map->getTiles()[this->getMapPosition().y][this->getMapPosition().x].name = NOTHING;
            map->getTiles()[this->getMapPosition().y][this->getMapPosition().x].address = NULL;
            map->getTiles()[futureMapPosition.y][futureMapPosition.x].name = HERO;
            map->getTiles()[futureMapPosition.y][futureMapPosition.x].address = this;
            this->setMapPosition(futureMapPosition.x, futureMapPosition.y);
            if(tileName == TRAP){
                std::cout << "You are stuck in a trap. You'll skip your next turn\n";
                map->nextTurn();
                std::cout << "You skip your turn.\n";
            }
            break;
        case BLOCK:
            std::cout << "You bumped into a terrain. Choose the other way.\n";
            this->makeTurn(map);
            break;
        case ENEMY:
            enemy* enemy;
            enemy = (class enemy*) map->getTiles()[futureMapPosition.y][futureMapPosition.x].address;
            if(this->getStance()){
                enemy->setHP(enemy->getHP()+this->getAttackDmg()/2);
            }
            enemy->setHP(enemy->getHP()-this->getAttackDmg());
            std::cout << "You attack the Enemy. Now it has " << enemy->getHP() << "HP\n";
            if(enemy->getHP() <= 0){
                class enemy* tmp;
                map->getTiles()[enemy->getMapPosition().y][enemy->getMapPosition().x].name = NOTHING;
                for(std::vector<class enemy*>::iterator it = map->getEnemies().begin() + enemy->getId() + 1; it < map->getEnemies().end(); it++){
                    tmp = *it;
                    tmp->setId(tmp->getId()-1);
                }
                map->getEnemies().erase(map->getEnemies().begin() + enemy->getId());
                delete enemy;
                std::cout << "You defeated the Enemy\n";
            }
            break;
        case CASTLE:
            castle* enemyCastle;
            enemyCastle = (class castle*) map->getTiles()[futureMapPosition.y][futureMapPosition.x].address;
            enemyCastle->setHP(enemyCastle->getHP()-this->getAttackDmg());
            std::cout << "You attack the Castle. Now it has " << enemyCastle->getHP() << "HP\n";
            if(enemyCastle->getHP() <= 0){
                castle* tmp;
                map->getTiles()[enemyCastle->getMapPosition().y][enemyCastle->getMapPosition().x].name = NOTHING;
                for(std::vector<castle*>::iterator it = map->getCastles().begin() + enemyCastle->getId() + 1; it < map->getCastles().end(); it++){
                    tmp = *it;
                    tmp->setId(tmp->getId()-1);
                }
                map->getCastles().erase(map->getCastles().begin() + enemyCastle->getId());
                delete enemyCastle;
                std::cout << "You destroyed the Castle\n";
            }
            break;
        default:
            std::cout << "You can't move here. Choose the other way.\n";
            this->makeTurn(map);
    }
}

hand& hero::getInventory()
{
    return this->inventory;
}

void hero::makeTurn(map* map)
{
    this->setMP(this->getMP()+MANAREGEN);
    coords futureMapPosition = this->getMapPosition();
    char command = getACharacter(); 
    size_t targetNumber;
    int index = 0;
    switch(command){
        case 'W':
        case 'w':
            if(futureMapPosition.y){
                futureMapPosition.y--;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 'A':
        case 'a':
            if(futureMapPosition.x){
                futureMapPosition.x--;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 'S':
        case 's':
            if(futureMapPosition.y + 1 != map->getHeight()){
                futureMapPosition.y++;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 'D': 
        case 'd':
            if(futureMapPosition.x + 1 != map->getLength()){
                futureMapPosition.x++;
                this->environmentInteraction(map, futureMapPosition);
            }else{
                this->makeTurn(map);
            }
            break;
        case 'R':
        case 'r':
            this->changeStance();
            std::cout << "You are changing your weapon. You'll skip your next turn\n";
            map->nextTurn();
            std::cout << "You skip your turn.\n";
            break;
        case 'Q':
        case 'q':
            {
            if(!stance){
                std::cout << "You are not in range stance!\n";
                this->makeTurn(map);
                break;
            }
            std::cout << "Choose a target within " << FIRERANGE << " tile range (a number of the target is expected):\n";
            coords heroPosition = this->getMapPosition();
            std::vector<enemy*> targets;
            coords xPosition;
            for(enemy* x : map->getEnemies()){
                xPosition = x->getMapPosition();
                if(abs(heroPosition.x-xPosition.x)<= FIRERANGE && 
                abs(heroPosition.y-xPosition.y) <= FIRERANGE){
                    targets.push_back(x);
                    std::cout << index++ << ". Enemy at (" << xPosition.x << ", " << xPosition.y << ")\n";
                }
            }
            if(index){
                targetNumber = this->getANumber() - 1;
                if(targetNumber >= 0 && targetNumber < index){
                    if(this->getStance()){
                        environmentInteraction(map, targets[targetNumber]->getMapPosition());
                    }
                }else{
                    std::cout << "No target with such a number\n";
                    this->makeTurn(map);
                }
            }else{
                std::cout << "No target within range\n";
                this->makeTurn(map);
            }
            break;
            }
        case 'I':
        case 'i':
            std::cout << "Your inventory:\n";
            for(int i = 0; i < this->getInventory().getSpellList().size(); i++){
                std::cout << i+1 << ". " << this->getInventory().getSpellList().at(i)->getName() << ", " << this->getInventory().getSpellList().at(i)->getRank() << '\n';
                index++;
            }
            targetNumber = this->getANumber() - 1;
            if(targetNumber >= 0 && targetNumber < index){
                this->getInventory().getSpellList().at(targetNumber)->use(map, this);
            }else{
                std::cout << "No item with such a number\n";
                this->makeTurn(map);
            }
            break;
        case 'C':
        case 'c':
            if(this->getMP() >= MANACOST){
                if(!(this->getInventory().getSpellList().size() == this->getInventory().getSpellListLimit())){
                    if(this->getInventory().craft(this)){
                        this->makeTurn(map);
                    }
                }else{
                    std::cout << "Can't craft - need more inventory space (6 spells max)\n";
                    this->makeTurn(map);
                }
            }else{
                std::cout << "Can't craft - need more mana\n";
                this->makeTurn(map);
            }
            break;
        case 'H':
        case 'h':
            checkYourself();
            makeTurn(map);
            break;
        case '`':
            changeMenuFlag();
            break;
        case ' ':
            std::cout << "You decided to skip your turn\n";
            break;
        default:
            std::cout << "Hero moves via WASD buttons or SPACE to skip a turn. Press Q to use Skill, R to change Stance.\nPlease enter again\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            this->makeTurn(map);
    }
}

void hero::buyUpgrades(map* map)
{
    size_t command;
    size_t x;
    size_t y;
    coords futureMapPosition;
    for(size_t i = 0; i < map->getDifficultyLvL()-1; i++){
        std::cout << "\tHero upgrade table. You have " << map->getDifficultyLvL()-i << "choice(s)\n";
        std::cout << "1. Healthy food: " << this->getMaxHP() << "HP -> " << this->getMaxHP() + 10 << "HP\n";
        std::cout << "2. A better weapon: " << this->getAttackDmg() << " Melee Attack -> " << this->getAttackDmg() + 5 << " Melee attack\n";
        std::cout << "3. Magical herb: " << this->getMP() << "MP -> " << this->getMP() + 30 << "MP\n";
        std::cout << "4. Teleport (can start a fight) from Current Position (x, y) = (" << this->getMapPosition().x << ", " << this->getMapPosition().y << ")\n";
        std::cout << "5. More ancient scrolls: Upgrade your rank of your spells\n";
        std::cout << "6. Skip\n";
        command = 0;
        command = getANumber();
        switch(command){
            case 1:
                std::cout << "You feel yourself healthier\n";
                setMaxHP(this->getMaxHP() + 10);
                setHP(getMaxHP());
                break;
            case 2:
                std::cout << "You feel yourself mightier\n";
                setAttackDmg(this->getAttackDmg() + 5);
                break;
            case 3:
                std::cout << "You feel magic overflow\n";
                setMP(this->getMP() + 30 + 5*(map->getDifficultyLvL()-1));
                break;
            case 4:
                std::cout << "You are using teleport. Where do you want to go? (x,y)\n";
                futureMapPosition.x = getANumber();
                futureMapPosition.y = getANumber();
                if(futureMapPosition.x < map->getLength() && futureMapPosition.y < map->getHeight()){
                    this->environmentInteraction(map, futureMapPosition);
                }else{
                    std::cout << "Oops! Should've been more cautious with magic! It doesn't work where you don't see\n";
                }
                break;
            case 5:
                std::cout << "You feel yourself more wizardry than ever!\n";
                setMagicalMastery(getMagicalMastery()+1);
                break;
            case 6:
                break;
            default:
                i++;
        }
    }
}

void hero::checkYourself()
{
    std::cout << "\tHero info table\n";
    std::cout << "HP = " << this->getHP() << "\nMelee Attack = " << this->getAttackDmg() << '\n';
    std::cout << "MP = " << this->getMP() << "\nCurrent Stance = " << (this->getStance()? "Ranged\n" : "Melee\n");
    std::cout << "Current Position (x, y) = (" << this->getMapPosition().x << ", " << this->getMapPosition().y << ")\n";
    std::cout << "Inventory:\n";
    for(auto x : this->getInventory().getSpellList()){
        std::cout << x->getName() << " rank " << x->getRank() << '\n';
    }
}

hero::hero(int HP, size_t attackDmg, size_t MP) : MP(MP), stance(0), menuFlag(0), maxHP(HP), magicalMastery(1)
{
    this->setHP(HP);
    this->setAttackDmg(attackDmg);
    this->setId(0);
    this->setMapPosition(0, 0);
    this->inventory = hand();
}

hero::hero(int HP, size_t attackDmg) : hero(HP, attackDmg, STDMP){}

hero::hero(int HP) : hero(HP, STDATTACK){}

hero::hero() : hero(STDHP){}

void hero::changeStance()
{
    this->stance = !stance;
}

void hero::setStance(size_t newStance)
{
    this->stance = newStance;
}

size_t hero::getStance()
{
    return this->stance;
}

void hero::setMP(size_t MP)
{
    this->MP = MP;
}

size_t hero::getMP()
{
    return this->MP;
}

void hero::setMaxHP(size_t newMaxHP)
{
    this->maxHP = newMaxHP;
}

size_t hero::getMaxHP()
{
    return this->maxHP;
}

void hero::changeMenuFlag()
{
    this->menuFlag = !menuFlag;
}

size_t hero::getMenuFlag()
{
    return this->menuFlag;
}

void hero::setMagicalMastery(size_t rank)
{
    this->magicalMastery = rank;
}

size_t hero::getMagicalMastery()
{
    return this->magicalMastery;
}

hero::~hero()
{
    for(auto x : getInventory().getSpellList()){
        delete x;
    }
    
}
