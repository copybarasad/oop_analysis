#pragma once
#include <cstdlib>
#include <string>


class Game;

class ParticleHelper {
public:
    explicit ParticleHelper(Game &game): game_(game) {
    }

    void spawn_blood_effect(float world_x, float world_y);

    void spawn_death_effect(float world_x, float world_y);

    void spawn_click_effect(float world_x, float world_y);

    void spawn_lava_effect(float world_x, float world_y);

    void spawn_projectile(const std::string& texture,float ttl, float size, float src_x, float src_y, float dst_x, float dst_y);

private:
    Game &game_;

    float random_float(const float min, const float max) const {
        return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
    }
};
