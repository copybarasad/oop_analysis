#pragma once
#include "characters/enemy.h"
#include "characters/enemyHut.h"
#include "characters/tower.h"
#include "spellManager.h"
#include <random>
#include <algorithm>
#include "configs/enemyConf.h"


class EnemyManager{
    private:
        std::vector<std::unique_ptr<Enemy>> enemies_;
        std::vector<std::unique_ptr<EnemyHut>> enemyHuts_;
        std::vector<std::unique_ptr<Tower>> towers_;
        en_conf conf;

        void generateEnemyHut(Field &field_, int numEnemyHut);
        void generateEnemy(Field &field_, int numEnemy);
        void generateTower(Field &field_, int numTower);

    public:
        EnemyManager(Field& field);
        EnemyManager();

        void enemyMove(Player &player, Field &field, SpellManager &trap);
        void enemyHutsMove(Player &player_, Field &field_);
        void towerMove(Player &player, Field &field);
        void setDamage(Position pos, int damage);
        
        const std::vector<std::unique_ptr<Enemy>>& getEnemy() const ;
        const std::vector<std::unique_ptr<EnemyHut>>& getEnemyHut() const ;
        const std::vector<std::unique_ptr<Tower>>& getTowers() const ;

        void generateEnemies(Field& field);

        void improveEnemies();

        bool enemyEmpty();

        void addEnemy(std::unique_ptr<Enemy> en);
        void addEnemyHut(std::unique_ptr<EnemyHut> enH);
        void addTower(std::unique_ptr<Tower> tw);
        void cleanEnemies();

       
};