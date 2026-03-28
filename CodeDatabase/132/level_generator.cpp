#include "level_generator.hpp"
#include "assert.h"

FieldHandler Level::get_lvl(int lvl) {
    gen.seed(seed + lvl);

    FieldHandler field_handler(gen() % 16 + 10, gen() % 16 + 10);

    gen_landscape(field_handler);
    gen_entities(field_handler, lvl);

    return field_handler;
}

void Level::gen_landscape(FieldHandler& field_handler) {
    std::uniform_real_distribution<double> prob(0.0, 1.0);

    auto place_rounded_stone_block = [&](int cx, int cy, int R) {
        for (int y = cy - R; y <= cy + R; ++y) {
            for (int x = cx - R; x <= cx + R; ++x) {
                if (x < 0 || y < 0 || x >= field_handler.width() || y >= field_handler.height()) continue;
                double d = std::sqrt(double(x - cx) * (x - cx) + double(y - cy) * (y - cy));
                if (d <= R * (1.0 + 0.3 * (prob(gen) - 0.5))) {
                    GameObject* obj = new Stone();
                    field_handler.place_landscape_obj({ y, x }, obj);
                }
            }
        }
    };

    int maxR = std::max(3, std::min(field_handler.height(), field_handler.width()) / 2);
    std::uniform_int_distribution<int> rdist(std::max(2, maxR / 2), maxR);

    place_rounded_stone_block(field_handler.width() - 1, 0, rdist(gen));
    place_rounded_stone_block(0, field_handler.height() - 1, rdist(gen));

    for (auto [sy, sx] : field_handler.get_positions('#')) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 && dy == 0) continue;
                int nx = sx + dx, ny = sy + dy;
                if (nx < 0 || ny < 0 || nx >= field_handler.width() || ny >= field_handler.height()) continue;
                if (field_handler.get_obj({ny, nx}) == nullptr && prob(gen) < 0.85) {
                    GameObject* obj = new Swamp();
                    field_handler.place_landscape_obj({ ny, nx }, obj); // TODO: place_swamp, place_stone
                }
            }
        }
    }

    int cx = field_handler.width() / 2;
    int cy = field_handler.height() / 2;
    int R = std::max(2, std::min(field_handler.height(), field_handler.width()) / 5);
    for (int y = cy - R; y <= cy + R; ++y) {
        for (int x = cx - R; x <= cx + R; ++x) {
            if (x < 0 || y < 0 || x >= field_handler.width() || y >= field_handler.height()) continue;
            double d = std::sqrt(double(x - cx) * (x - cx) + double(y - cy) * (y - cy));
            double p = std::max(0.25, 1.0 - d / (R + 0.1));
            if (field_handler.get_obj({y, x}) == nullptr && prob(gen) < p) {
                GameObject* obj = new Swamp();
                field_handler.place_landscape_obj({ y, x }, obj);
            }
        }
    }

    double stone_chance = 0.02;
    double swamp_chance = 0.04;

    for (int y = 0; y < field_handler.height(); ++y) {
        for (int x = 0; x < field_handler.width(); ++x) {
            if (field_handler.get_obj({y, x}) != nullptr) continue;
            double r = prob(gen);
            if (r < stone_chance) {
                GameObject* obj = new Stone();
                field_handler.place_landscape_obj({ y, x }, obj);
            }

            else if (r < stone_chance + swamp_chance) {
                GameObject* obj = new Swamp();
                field_handler.place_landscape_obj({ y, x }, obj);
            }
        }
    }
}

std::set<Position> Level::gen_positions(std::vector<Position> free_cells, int count) {
    std::set<Position> positions;

    assert(free_cells.size() >= count);

    std::shuffle(free_cells.begin(), free_cells.end(), gen);

    for (int i = 0; i < count; i++) {
        positions.insert(free_cells[i]);
    }

    return positions;
}

Position Level::gen_player_pos(FieldHandler& field_handler, int difficulty) {
    return { 1, 1 };
}

std::set<Position> Level::gen_enemy_spawners_pos(FieldHandler& field_handler, int difficulty) {
    int h = field_handler.height();
    int w = field_handler.width();

    return gen_positions(
        field_handler.free_cells(
            { (h * 1) / 3, (w * 1) / 3 }, 
            { h - 1, w - 1 }
        ),
        difficulty
    );
}

std::set<Position> Level::gen_enemy_towers_pos(FieldHandler& field_handler, int difficulty) {
    int h = field_handler.height();
    int w = field_handler.width();

    return gen_positions(
        field_handler.free_cells(
            { (h * 2) / 3, (w * 2) / 3 }, 
            { h - 1, w - 1 }
        ),
        difficulty
    );
}

std::set<Position> Level::gen_enemies_pos(FieldHandler& field_handler, int difficulty) {
    int h = field_handler.height();
    int w = field_handler.width();
    
    return gen_positions(
        field_handler.free_cells(
            { (h * 2) / 3, (w * 2) / 3 }, 
            { h - 1, w - 1 }
        ),
        difficulty * 3
    );
}

void Level::place_enemies(FieldHandler& field_handler, const std::set<Position>& positions, const MobImage& img) {
    for (const Position& pos: positions) {
        field_handler.place_img(pos, img, 'E');
    }
}

void Level::place_enemy_towers(FieldHandler& field_handler, const std::set<Position>& positions, const BuildingImage& img) {
    for (const Position& pos: positions) {
        field_handler.place_img(pos, img, 'T');
    }
}

void Level::place_enemy_spawners(FieldHandler& field_handler, const std::set<Position>& positions, const BuildingImage& img) {
    for (const Position& pos: positions) {
        field_handler.place_img(pos, img, 'B');
    }
}

void Level::gen_entities(FieldHandler& field_handler, int difficulty) {
    field_handler.place_img(
        gen_player_pos(field_handler, difficulty), 
        PlayerImage(
            100,
            true,
            0,
            HandImage(
                2,
                0,
                ""
            ),
            {}
        )
    );

    place_enemy_spawners(
        field_handler,
        gen_enemy_spawners_pos(field_handler, difficulty),
        BuildingImage(
            difficulty * 20,
            10,
            0,
            {}
        )
    );

    place_enemy_towers(
        field_handler,
        gen_enemy_towers_pos(field_handler, difficulty),
        BuildingImage(
            difficulty * 20,
            10,
            0,
            {}
        )
    );

    place_enemies(
        field_handler,
        gen_enemies_pos(field_handler, difficulty),
        MobImage(
            difficulty * 10,
            true,
            {}
        )
    );
}