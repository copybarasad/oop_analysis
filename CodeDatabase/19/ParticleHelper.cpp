#include "ParticleHelper.h"

#include "core/Game.h"


void ParticleHelper::spawn_blood_effect(float world_x, float world_y) {
    for (int i = 0; i < 5; ++i) {
        float vx = random_float(-2.0f, 1.0f);
        float vy = random_float(-2.0f, 1.0f);

        float size = random_float(4.0f, 12.0f);
        float ttl = random_float(0.1f, 0.3f);

        game_.spawn_particle(Particle(
            "p_blood",
            ttl,
            true,
            size,
            {220, 255, 255, 255},
            {100, 0, 0, 200},
            {world_x + random_float(-0.5, 0.2), world_y + random_float(-0.5, 0.2)},
            {vx, vy},
            {2.0f, 2.0f},
            {0.9f, 0.9f}
        ));
    }
}

void ParticleHelper::spawn_death_effect(float world_x, float world_y) {
    for (int i = 0; i < 25; ++i) {
        float vx = random_float(-2.0f, 1.0f);
        float vy = random_float(-2.0f, 1.0f);

        float size = random_float(4.0f, 12.0f);
        float ttl = random_float(0.1f, 0.3f);

        game_.spawn_particle(Particle(
                "p_blood",
                ttl,
                true,
                size,
                {220, 100, 100, 255},
                {100, 0, 0, 255},
                {world_x + random_float(-0.2, 0.2), world_y + random_float(-0.2, 0.2)},
                {vx, vy},
                {5.0f, 5.0f},
                {0.97f, 0.97f}
        ));
    }
}


void ParticleHelper::spawn_lava_effect(float world_x, float world_y) {
    for (int i = 0; i < 10; ++i) {
        float vx = random_float(-2.0f, 0.3f);
        float vy = vx;

        float size = random_float(4.0f, 12.0f);
        float ttl = random_float(0.3f, 0.5f);

        game_.spawn_particle(Particle(
            rand() % 2 == 0 ? "p_flame" : "p_lava",
            ttl,
            true,
            size,
            {255, 255, 255, 255},
            {100, 0, 0, 200},
            {world_x + random_float(-0.5, 0.5) - 0.25, world_y + random_float(-0.5, 0.5) - 0.25},
            {vx, vy},
            {4.0f, 4.0f},
            {0.9f, 0.9f}
        ));
    }
}

void ParticleHelper::spawn_projectile(const std::string &texture, float ttl, float size, float src_x, float src_y, float dst_x, float dst_y) {
    float dx = dst_x - src_x;
    float dy = dst_y - src_y;

    game_.spawn_particle(Particle(
            texture,
            ttl,
            true,
            size,
            {255, 255, 255, 255},
            {255, 255, 255, 100},
            {src_x, src_y},
            {dx/ttl,dy/ttl},
            {0, 0},
            {0,0}
    ));
}
