#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <memory>
#include "player.h"
#include "enemy.h"
#include "building.h"
#include "place.h"
#include "spell_tower.h"
#include <string>
#include <nlohmann/json.hpp>
#include "random.h"


#define PLACE_EMPTY -1
#define PLACE_PLAYER 0
#define PLACE_ENEMY 1
#define PLACE_BUILDING 2
#define PLACE_SPELL_TOWER 3

#define EFFECT_FREEZE "freeze"
#define EFFECT_WALL "wall"
#define EFFECT_NONE "no"

#define COMBAT_MELEE "melee"
#define COMBAT_RANGED "ranged"
#define COMBAT_NONE "none"

#define PLAYER_HP 100
#define PLAYER_MELEE_DAMAGE 30
#define PLAYER_RANGED_DAMAGE 10
#define PLAYER_POINTS 0

#define ENEMY_HP 12
#define ENEMY_DAMAGE 2
#define ENEMY_STRENGTH 2

#define BUILDING_HP 5
#define BUILDING_COOLDOWN 3
#define BUILDING_SPAWN_TIMER 0

#define MIN_FREEZE_PLACE 10
#define MAX_FREEZE_PLACE 18
#define MIN_WALL_PLACE 10
#define MAX_WALL_PLACE 40
#define MIN_ENEMIES 1
#define MAX_ENEMIES 5
#define MIN_BUILDINGS 0
#define MAX_BUILDINGS 2
#define MIN_SPELL_TOWER 0
#define MAX_SPELL_TOWER 2

#define SPELL_TOWER_HP 55 
#define SPELL_TOWER_RANGED 5
#define SPELL_TOWER_TIME 3

#define PLAYER_MANA 50

#define DEFAULT_WIDTH 25
#define DEFAULT_HEIGHT 15

#define MELEE_RANGE 1
#define RANGED_RANGE 5

class Field {
private:
    int width;
    int height;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::unique_ptr<Player> gamer;
    std::vector<std::vector<Place>> table;
    std::vector<std::unique_ptr<SpellTower>> spell_towers;

    
public:
    Field(int w, int h);
    Field();
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field& other);
    Field& operator=(Field& other);
    
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    
    const std::vector<std::unique_ptr<Enemy>>& getEnemies();
    int getCntEnemy();
    void addEnemy(std::unique_ptr<Enemy> fighter);
    Building* getBuild(int id);
    Player* getPlayer();
    SpellTower* getSpellTower(int id);
    int getCntBuilding();
    int getWidth();
    int getHeight();
    bool Place_is_valid(int new_x, int new_y);
    void setPlayer(std::unique_ptr<Player> fighter);
    std::string getEffectPlace(int x, int y);
    int getCntSpellTower();
    void delEffectPlace(int x, int y);
    void setTypePlace(int x, int y, int type);
    int getTypePlace(int x, int y);
    void switch_ranged();
    void switch_melee();
    void initilization_field(int parametr);
    bool attack_player(int dx, int dy);
    void removeEnemy(int index);
    void removeBuilding(int index);
    void setEffectPlace(int x, int y, std::string ef);
    void print_field();
    void removeSpellTower(int index);
};

#endif