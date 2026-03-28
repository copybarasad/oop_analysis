
#include "spawn_system.h"

bool SpawnSystem::spawn_enemy(Building& build, Field& field) {
    if (build.getSpawn() < build.getCooldown()) {
        build.setSpawnTimer(build.getSpawn()+1);
        return true;
    }

    int width = field.getWidth();
    int height = field.getHeight();

    int x_build = build.getX();
    int y_build = build.getY();
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int new_x = x_build+j;
            int new_y = y_build+i;
            if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
                continue;
            }

            if (field.Place_is_valid(new_x, new_y)) {
                field.addEnemy(std::make_unique<Enemy>(new_x, new_y, 10, 3, 3));
                build.setSpawnTimer(BUILDING_SPAWN_TIMER);
                return true;
            }
        }
    }
    return false;
}
