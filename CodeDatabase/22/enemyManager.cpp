#include "managment/enemyManager.h"
#include "characters/player.h"
#include "field/field.h"
#include "field/position.h"
#include "configs/gameConf.h"
#define FALL Position {-1, -1}

EnemyManager::EnemyManager(Field& field){
    this->generateEnemies(field);
    
}
EnemyManager::EnemyManager(){}

void EnemyManager::enemyMove(Player& player, Field& field, SpellManager &trap){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-1, 1);

    conf_t gameConf;

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [&](const auto &en)
                       {
                           if (!en)
                               return true;
                           if (!en->isAlive())
                           {
                               field.deliteCharacter(en->getPosition());

                               if (player.getHealth() < gameConf.playerHealth)
                               {
                                   player.setHealth(player.getHealth() + 10);
                               }
                               player.scoreUp();
                               if ((player.getScore() / 10) % 2 == 0)
                               {
                                   player.getHand()->generateRandomSpell();
                               }
                               if (player.getScore() == 100)
                               {
                                   player.levelUp();
                               }

                               return true;
                           }
                           return false;
                       }),
        enemies_.end());

    for (auto &en : enemies_)
    {

        if (!en)
            continue;

        if (field.isTrap(en->getPosition()))
        {
            en->takeDamage(trap.getTrapDamage());
            field.deliteTrap(en->getPosition());
            trap.deliteTrap(en->getPosition());
        }

        Position playerPos = field.playerInZone(en->getPosition(), 1);

        if (playerPos != FALL)
        {
            en->attack(player);
        }
        else
        {
            Position newPos = en->getPosition();
            newPos.x += dist(gen);
            newPos.y += dist(gen);

            if (field.isFree(newPos))
            {
                field.deliteCharacter(en->getPosition());
                field.addEnemy(newPos);
                en->move(newPos);
            }
        }
    }
}

void EnemyManager::enemyHutsMove(Player& player_, Field& field_){
    conf_t gameConf;

    enemyHuts_.erase(
        std::remove_if(enemyHuts_.begin(), enemyHuts_.end(),
                       [&](const auto &enH)
                       {
                           if (!enH)
                               return true;
                           if (!enH->isAlive())
                           {
                               field_.deliteCharacter(enH->getPosition());

                               if (player_.getHealth() < gameConf.playerHealth)
                               {
                                   player_.setHealth(player_.getHealth() + 10);
                               }
                               player_.scoreUp();
                               if ((player_.getScore() / 10) % 2 == 0)
                               {
                                   player_.getHand()->generateRandomSpell();
                               }
                               if (player_.getScore() == 100)
                               {
                                   player_.levelUp();
                               }
                               return true;
                           }
                           return false;
                       }),
        enemyHuts_.end());

    for (auto &enH : enemyHuts_)
    {

        if (!enH)
            continue;

        if (enH->update())
        {
            enemies_.push_back(std::move(enH->generateEnemy()));
        }
    }
}
void EnemyManager::towerMove(Player& player, Field& field){
    for (auto &tw : towers_)
    {
        int damage = tw->attack()->use().first;
        int radius = tw->attack()->use().second;

        if (field.playerInZone(tw->getPosition(), radius) != FALL)
        {
            player.takeDamage(damage);
        }
    }
}

void EnemyManager::generateEnemy(Field& field_, int numEnemy)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numEnemy; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while (!field_.isFree(randPos))
        {
            randPos = {dist(gen), dist(gen)};
        }

        std::unique_ptr<Enemy> en = std::make_unique<Enemy>(conf.enemyHealth, conf.enemyDamage, randPos);
        field_.addEnemy(randPos);
        this->enemies_.push_back(std::move(en));
    }
}

void EnemyManager::generateEnemyHut(Field &field_, int numEnemyHut)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numEnemyHut; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while (!field_.isFree(randPos))
        {
            randPos = {dist(gen), dist(gen)};
        }

        field_.addEnemyHut(randPos);
        this->enemyHuts_.push_back(std::make_unique<EnemyHut>(conf.enemyHutHealth, randPos, 15));
    }
}

void EnemyManager::generateTower(Field &field_, int numTower)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, field_.getHeight() - 1);

    for (int i = 0; i < numTower; i++)
    {
        Position randPos = {dist(gen), dist(gen)};
        while (!field_.isFree(randPos))
        {
            randPos = {dist(gen), dist(gen)};
        }

        std::unique_ptr<Tower> tower = std::make_unique<Tower>(conf.towerHealth, randPos);
        field_.addTower(randPos);
        this->towers_.push_back(std::move(tower));
    }
}

void EnemyManager::generateEnemies(Field& field){
    generateEnemy(field, conf.numEnemy);
    generateEnemyHut(field, conf.numEnemyHut);
    generateTower(field, conf.numTower);
}

void EnemyManager::setDamage(Position pos, int damage){
    
    auto it = std::find_if(enemies_.begin(), enemies_.end(), [&](auto& en){return pos == en->getPosition();});
    if (it == enemies_.end()){
        auto itH = std::find_if(enemyHuts_.begin(), enemyHuts_.end(), [&](auto& enH){return pos == enH->getPosition();});

        if (itH == enemyHuts_.end()) return;
        else{
            (*itH)->takeDamage(damage);
        }
    }
    else{
        (*it)->takeDamage(damage);
    }
}

void EnemyManager::improveEnemies(){
    conf.enemyHealth+=10;
    conf.enemyDamage+=5;
    conf.numEnemy+=2;

    conf.numEnemyHut+=2;
    conf.numTower+=2;

    conf.towerHealth+=10;
    conf.towerRadius+=1;
}


bool EnemyManager::enemyEmpty(){
    return enemies_.empty() && enemyHuts_.empty();
}

void EnemyManager::addEnemy(std::unique_ptr<Enemy> en){
    enemies_.push_back(std::move(en));
}
void EnemyManager::addEnemyHut(std::unique_ptr<EnemyHut> enH){
    enemyHuts_.push_back(std::move(enH));
}
void EnemyManager::addTower(std::unique_ptr<Tower> tw){
    towers_.push_back(std::move(tw));
}
void EnemyManager::cleanEnemies(){
    enemies_.clear();
    enemyHuts_.clear();
    towers_.clear();
}

const std::vector<std::unique_ptr<Enemy>>& EnemyManager::getEnemy() const{
    return enemies_;
}
const std::vector<std::unique_ptr<EnemyHut>>& EnemyManager::getEnemyHut() const{
    return enemyHuts_;
}
const std::vector<std::unique_ptr<Tower>>& EnemyManager::getTowers() const{
    return towers_;
}