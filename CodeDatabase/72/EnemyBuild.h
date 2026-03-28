//
// Created by bob on 10/5/25.
//

#ifndef GAME_TERM_ENEMYBUILD_H
#define GAME_TERM_ENEMYBUILD_H
#include "Character.h"
#include "Enemy.h"

#define DEFAULT_ENEMIES_HEALTH 100
#define DEFAULT_ENEMIES_DAMAGE 20
#define DEFAULT_ENEMY_BUILD_HEALTH 200
#define DEFAULT_SPAWN_COUNT 10
class EnemyBuild: public Character, public ISerializable{

   int spawnCount;
   int enemiesHealth;
   int enemiesDamage;

   int validatePositive(const int & value, const std::string & error) const;

public:

   explicit EnemyBuild(const int & enemiesHealth = DEFAULT_ENEMIES_HEALTH, const int & enemiesDamage = DEFAULT_ENEMIES_DAMAGE,const int & health = DEFAULT_ENEMY_BUILD_HEALTH, const int & spawnCount = DEFAULT_SPAWN_COUNT);

   void setSpawnCount(const int & spawnCount);

   void setEnemiesHealth(const int & enemiesHealth);

   void setEnemiesDamage(const int & enemiesDamage);

   bool isSpawn(const int & counterSteps) const;

   Enemy spawnEnemy() const;

   void save(Saver &saver) const override;

   void load(Reader &reader) override;

};


#endif //GAME_TERM_ENEMYBUILD_H