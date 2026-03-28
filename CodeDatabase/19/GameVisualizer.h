#pragma once

#include <map>

#include "particle/Particle.h"
#include "renderer/IRenderer.h"
#include "world/World.h"
class Game;


template <typename RenderImpl>
class GameVisualizer {




private:
    Game& game_;
    RenderImpl renderer_;
    std::vector<Particle> particles_;
    int last_window_width_ = 0;
    int last_window_height_ = 0;

public:
    explicit GameVisualizer(Game &game)
        : game_(game),
          renderer_() {
    }

    bool init(const std::string& title);

    void render(float dt);

    IRenderer& renderer() {
        return renderer_;
    }

    void destroy();

    void add_particle(const Particle & particle) {
        particles_.push_back(std::move(particle));
    }

private:
    void load_fonts() {
        static const std::map<std::string, std::pair<std::string, int>> fonts = {
            {"default", {"assets\\fonts\\PixeloidSans.ttf", static_cast<int>(12 * GUI_SCALE)}},
            {"default_large", {"assets\\fonts\\PixeloidSans.ttf", static_cast<int>(24 * GUI_SCALE)}}
        };

        for (const auto& [name, info] : fonts) {
            renderer_.load_font(name, info.first, info.second);
        }
    }

    void load_textures() {
        static const std::map<std::string, std::string> textures = {
            {"p_lava", "assets\\particle\\lava.png"},
            {"p_blood", "assets\\particle\\blood.png"},
            {"p_stone", "assets\\particle\\stone.png"},
            {"p_flame", "assets\\particle\\flame.png"},
            {"grass", "assets\\grass.png"},
            {"lava", "assets\\lava.png"},
            {"mud", "assets\\mud.png"},
            {"water", "assets\\water.png"},
            {"ice", "assets\\ice.png"},
            {"player", "assets\\player.png"},
            {"player_ranged", "assets\\player_ranged.png"},
            {"enemy", "assets\\enemy.png"},
            {"enemy_house", "assets\\enemy_house.png"},
            {"enemy_tower", "assets\\enemy_tower.png"},
            {"ally", "assets\\ally.png"},
            {"trap", "assets\\trap.png"},
            {"win_screen", "assets\\win_screen.png"},
            {"lose_screen", "assets\\lose_screen.png"},
            {"spell_selection", "assets\\spell_selection.png"},
            {"tile_selection", "assets\\tile_selection.png"},
            {"invalid_tile_selection", "assets\\invalid_tile_selection.png"},
            {"spell_damage", "assets\\spell_damage.png"},
            {"spell_aoe_damage", "assets\\spell_aoe_damage.png"},
            {"spell_trap", "assets\\spell_trap.png"},
            {"spell_upgrade", "assets\\spell_upgrade.png"},
            {"spell_summon", "assets\\spell_summon.png"},
            {"player_upgrade", "assets\\player_upgrade.png"},
            {"restart_button", "assets\\reset_button.png"},
            {"save_button", "assets\\save_button.png"},
            {"load_button", "assets\\load_button.png"},
            {"next_level_button", "assets\\next_level_button.png"}
        };

        for (const auto& [name, path] : textures) {
            renderer_.load_texture(name, path);
        }
    }

    void update_camera(float dt);

    void render_world(float dt);

    void render_hud();

    void render_overlay();

    void render_spell_holder();

    void render_spell_preview();

    void render_particles(double dt);
};



