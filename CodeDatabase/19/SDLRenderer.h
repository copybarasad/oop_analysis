#pragma once

#include <functional>
#include <map>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "SDLWindow.h"
#include "core/render/renderer/IRenderer.h"

class SDLRenderer : public IRenderer {
public:
    SDLRenderer() = default;

    ~SDLRenderer() override;

    int window_width() const override;
    int window_height() const override;

    bool init(std::string title, int width, int height) override;

    bool load_texture(const std::string &id, const std::string &path) override;

    bool tint_texture(const std::string &id, const Color& tint = {255,255,255,255}) const override;
    bool draw_texture_centered(const std::string &id, int x, int y, int w, int h = -1) const override;
    bool draw_texture(const std::string &id, int x, int y, int w, int h = -1) const override;

    bool load_font(const std::string &id, const std::string &path, int size) override;

    int font_height(const std::string &id) const override;

    bool draw_text(const std::string &text, int x, int y, Color color, const std::string &font_id = "default", const TextAlign align = TextAlign::Left) const override;

    int text_height(const std::string &text, const std::string &font_id = "default") const override;
    int text_width(const std::string &text, const std::string &font_id = "default") const override;

    void clear() const override;

    void present() override;

    void set_draw_color(Color color) const override;

    void draw_rect(int x, int y, int w, int h, bool filled = false) const override;

    void draw_line(int x1, int y1, int x2, int y2) const override;

    void depth_render(double depth, std::function<void(const IRenderer&)> render_func) override;

    void destroy() override;

private:
    SDLWindow window;
    SDL_Renderer *renderer_ = nullptr;
    std::map<std::string, SDL_Texture *> textures_;
    std::map<std::string, TTF_Font *> fonts_;
    std::multimap<double, std::function<void(IRenderer&)>> depth_queue_;
};
