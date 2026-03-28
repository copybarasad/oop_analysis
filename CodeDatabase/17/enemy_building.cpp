#include "enemy_building.h"
#include "game_map.h"    
#include "enemy.h"       
#include "map_cell.h"    

const Position& EnemyBuilding::getPosition() const { return position; }

EnemyBuilding::EnemyBuilding(int x, int y, int level, int interval, int enemyHealth, int enemyDamage) 
    : position(x, y), 
      level(level),
      count(0), 
      interval(interval),
      spawnedEnemyHealth(enemyHealth),
      spawnedEnemyDamage(enemyDamage) {}

void EnemyBuilding::spawn(std::vector<Enemy*>& enemies, GameMap& map){
    count++;

    if(count >= interval){
        int SPAWN_DIRECTIONS[TOTAL_SPAWN_DIRECTIONS][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},          {0, 1},
            {1, -1},  {1, 0}, {1, 1}
        };

        for(int i = TOTAL_SPAWN_DIRECTIONS - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(SPAWN_DIRECTIONS[i][0], SPAWN_DIRECTIONS[j][0]);  
            std::swap(SPAWN_DIRECTIONS[i][1], SPAWN_DIRECTIONS[j][1]);
        }

        for(int i = 0; i < TOTAL_SPAWN_DIRECTIONS; i++){
            int new_x = position.getX() + SPAWN_DIRECTIONS[i][0];
            int new_y = position.getY() + SPAWN_DIRECTIONS[i][1];
            Position spawn_pos(new_x, new_y);

            if (map.isPositionValid(spawn_pos)) {
                MapCell& cell = map.getCell(spawn_pos);
                if (cell.getType() == MapCell::Type::EMPTY && !cell.isUsed()) {   
                    cell.setUsed(true);  
                    Enemy* enemy = new Enemy(new_x, new_y, level, spawnedEnemyHealth, spawnedEnemyDamage);   
                    enemies.push_back(enemy);
                    cell.setEntity(enemy);   
                    count = 0;
                    break;
                }
            }
        }
    }
}