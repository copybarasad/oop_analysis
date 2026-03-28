#include "movement_system.h"


bool MovementSystem::moves_game_object(GameObject& object, int new_x, int new_y, Field& field) {
    int startX = object.getX();
    int startY = object.getY();
    int types = field.getTypePlace(startX, startY);

    int dx = new_x - startX;
    int dy = new_y - startY;
    if (dx > 1 || dx < -1 || dy > 1 || dy < -1) {
        return false;
    }
    if (new_x >= field.getWidth() || new_x < 0 || new_y >= field.getHeight() || new_y < 0){
        return false;
    }

    if (Freezy_System::isFrozen(object)){
        Freezy_System::freezyReduceObject(object);
        return true;
    }


    if (field.Place_is_valid(new_x, new_y)){
        object.move(new_x - startX, new_y - startY);
        field.setTypePlace(new_x, new_y, types);
        field.setTypePlace(startX, startY, PLACE_EMPTY);
        if (field.getEffectPlace(new_x, new_y) == "freeze"){
            Freezy_System::freezyObject(object);
        } else if (field.getEffectPlace(new_x,new_y) == "trap"){
            object.takeDamage(30);
            int target_x = new_x;
            int target_y = new_y;
            const auto& enemies = field.getEnemies();
            for (size_t i = 0; i < enemies.size(); i++) {
                if (enemies[i]->getHP() <= 0) {
                    field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                    field.removeEnemy(i);
                }
            }

            field.delEffectPlace(new_x,new_y);
        }
        return true;
    } 
    return false;
}