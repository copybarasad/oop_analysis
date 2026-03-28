#include "core/Game.h"
#include "GameVisualizer.h"
#include "core/render/renderer/impl/SDLRenderer.h"
#include "spell/Spell.h"
#include "spell/SpellRegistry.h"

class SpellHolder;
class Player;

template<typename RenderImpl>
bool GameVisualizer<RenderImpl>::init(const std::string &title) {
    int max_dim = std::max(game_.world().width(), game_.world().height());

    int window_width = (max_dim + 2) * TILE_WIDTH;
    int window_height = (max_dim + 2) * TILE_HEIGHT + TILE_GROUND_HEIGHT;

    if(window_height > 720) window_height = 720;
    if(window_width > 1080) window_width = 1080;

    if (!renderer_.init(title, window_width, window_height)) {
        return false;
    }
    load_fonts();
    load_textures();

    return true;
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render(float dt) {

    renderer_.set_draw_color({20, 20, 28, 255});
    renderer_.clear();
    render_world(dt);
    render_hud();
    renderer_.depth_render(1000, [=, this](const IRenderer &renderer) {
        render_particles(dt);
    });
    renderer_.present();
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::destroy() {
    renderer_.destroy();
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_world(float dt) {
    World &world = game_.world();
    update_camera(dt);

    for (int y = 0; y < world.height(); ++y) {
        for (int x = 0; x < world.width(); ++x) {
            const WorldTile &tile = world.get_tile(x, y);
            renderer_.depth_render(x + y, [=, this](const IRenderer &r) {
                std::string texture_id = "grass";
                switch (tile.type()) {
                    case TileType::Ground:
                        break;
                    case TileType::Ice:
                        texture_id = "ice";
                        break;
                    case TileType::Lava:
                        texture_id = "lava";
                        break;
                    case TileType::Mud:
                        texture_id = "mud";
                        break;
                    default:
                        break;
                }

                const int screen_x = game_.world().get_screen_x(x, y);
                const int screen_y = game_.world().get_screen_y(x, y) + game_.world().get_tile_height_offset(x, y) * GUI_SCALE;
                (void) r.draw_texture_centered(texture_id, screen_x, screen_y, TILE_WIDTH);
            });
        }
    }

    for (const auto &item: world.entities()) {
        renderer_.depth_render(ceil(item->display_x()) + ceil(item->display_y()) + 0.5,
                               [=, this, &world](const IRenderer &r) {
                                   int center_x = world.get_screen_x(item->display_x(), item->display_y());
                                   int center_y = world.get_screen_y(item->display_x(), item->display_y());
                                   center_y += world.get_tile_height_offset(item->display_x(), item->display_y()) *
                                           GUI_SCALE;

                                   item->render(r, world, center_x, center_y);
                               });
    }
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_hud() {
    IRenderer &r = renderer();

    int w_width = r.window_width();
    int w_height = r.window_height();
    if (w_width != last_window_width_ || w_height != last_window_height_) {
        game_.on_window_resize();
        last_window_width_ = w_width;
        last_window_height_ = w_height;
    }

    render_spell_preview();
    r.depth_render(100, [=, this](const IRenderer &renderer) {
        render_overlay();
    });
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_overlay() {
    render_spell_holder();

    const IRenderer &renderer = renderer_;
    const GameLogic &logic = game_.logic();
    const World &world = game_.world();
    const Player &player = game_.player();

    std::string controls_string = R"(            Controls
[Space] Switch Weapon
[Mouse] Move
[1-9]   Select Spell)";

    (void) renderer.draw_text(controls_string,
                              last_window_width_ - renderer.text_width(controls_string) - 10 * GUI_SCALE,
                              last_window_height_ - renderer.text_height(controls_string) - 10 * GUI_SCALE,
                              {150, 150, 150, 255});

    int enemy_count = 0;

    for (const auto *entity: world.entities()) {
        if (entity->is_hostile()) {
            enemy_count++;
        }
    }

    std::string info_string = "Score: " + std::to_string(player.score()) + "\n"
                              + "Gold: " + std::to_string(player.get_attribute(Attribute::Gold)) + "\n"
                              + "Wave: " + std::to_string(logic.get_wave()) + "\n"
                              + "Your Health: " + std::to_string(player.get_attribute(Attribute::Health)) + "\n"
                              + "Enemies left: " + std::to_string(enemy_count);


    (void) renderer.draw_text(info_string, 10 * GUI_SCALE, 0, {230, 240, 255, 255});
    if (game_.state() == GameState::Finished) {
        std::string result = "Score: " + std::to_string(player.score());
        std::string font = "default_large";

        renderer.set_draw_color({0, 0, 0, 100});
        renderer.draw_rect(0, 0, last_window_width_, last_window_height_, true);

        int width = GUI_SCALE * 500;
        int height = GUI_SCALE * 87;

        int center_x = last_window_width_ / 2;
        int center_y = last_window_height_ / 2;
        (void) renderer.draw_texture(game_.player_won() ? "win_screen" : "lose_screen", center_x - width / 2,
                                     center_y - height, width);
        (void) renderer.draw_text(result, center_x - renderer.text_width(result, font) / 2, center_y,
                                  {255, 255, 255, 255},
                                  font);
    }


    for (auto &button: game_.hud().get_buttons()) {
        button.render(renderer);
    }
}


template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_spell_holder() {
    const IRenderer &renderer = renderer_;
    const SpellHolder &holder = game_.player().get_spell_holder();

    const int x_margin = GUI_SCALE * 10;
    const int y_margin = GUI_SCALE * 10;
    const int selection_x_offset = GUI_SCALE * 2;
    const int selection_y_offset = GUI_SCALE * 2;

    int x_offset = x_margin;
    const int width = 50 * GUI_SCALE;
    const int height = 75 * GUI_SCALE;
    const int y_pos = last_window_height_ - height - y_margin;


    const auto &spells = holder.get_spells();
    for (int i = 0; i < spells.size(); ++i) {
        const auto spell = SpellRegistry::instance().get(spells[i]);
        if (spell == nullptr) continue;
        bool selected = holder.get_selected_index() == i;
        if (selected && game_.logic().spell_focus()) {
            (void) renderer.draw_texture("spell_selection", x_offset - selection_x_offset,
                                         y_pos - selection_y_offset, width + selection_x_offset * 2);
        }
        renderer.draw_texture("spell_" + spell->id(), x_offset, y_pos, width);

        x_offset += width + static_cast<int>(GUI_SCALE * 10);
    }
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_spell_preview() {
    if (!game_.logic().spell_focus()) return;
    const SpellHolder &holder = game_.player().get_spell_holder();

    Spell *selected = holder.get_selected();
    if (selected != nullptr) {
        int mouse_x, mouse_y;
        game_.input().get_mouse_pos(mouse_x, mouse_y);

        selected->render_preview(renderer_, game_.world(), game_.player(), mouse_x,
                                 mouse_y, game_.logic().get_next_spell_level());
        renderer_.depth_render(1000, [=, this](const IRenderer &r) {
            r.draw_texture_centered("spell_" + selected->id(), mouse_x, mouse_y, GUI_SCALE * 10);
        });
    }
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::render_particles(const double dt) {
    for (auto &particle: particles_) {
        particle.tick(dt);

        std::pair<float, float> pos = particle.position();

        int screen_x;
        int screen_y;

        if (particle.world_relative()) {
            screen_x = game_.world().get_screen_x(pos.first, pos.second);
            screen_y = game_.world().get_screen_y(pos.first, pos.second);
        } else {
            screen_x = last_window_width_ * pos.first;
            screen_y = last_window_height_ * pos.second;
        }

        renderer_.tint_texture(particle.texture(), particle.current_color());
        renderer_.draw_texture_centered(particle.texture(), screen_x, screen_y, particle.size());
        renderer_.tint_texture(particle.texture());
    }

    std::erase_if(particles_, [](const Particle &p) {
        return p.dead();
    });
}

template<typename RenderImpl>
void GameVisualizer<RenderImpl>::update_camera(float dt) {
    World& world= game_.world();

    float px = game_.player().x();
    float py = game_.player().y();

    int width = world.width();
    int height = world.height();

    int pad = 5;

    if(px < pad) px = pad;
    if(py < pad) py = pad;
    if(px > (width - pad)) px = (width - pad);
    if(py > (height - pad)) py = (height - pad);






    float camX, camY;
    world.get_camera(camX, camY);

    int centerX = world.get_screen_x(px, py) + camX;
    int centerY = world.get_screen_y(px, py) + camY;






    float targetCamX = centerX - last_window_width_ / 2.0f;
    float targetCamY = centerY - last_window_height_ / 2.0f;

    if(camX == -1 && camY == -1){
        world.set_camera(targetCamX, targetCamY);
    }else {
        float min_move = 1;
        if(std::abs(camX - targetCamX) < min_move && std::abs(camY - targetCamY) < min_move) return;
        float cameraSpeed = 10.0f;

        float t = 1.0f - std::exp(-cameraSpeed * dt);

        float newCamX = camX + (targetCamX - camX) * t;
        float newCamY = camY + (targetCamY - camY) * t;

        world.set_camera(newCamX, newCamY);
    }

}

template class GameVisualizer<SDLRenderer>;
