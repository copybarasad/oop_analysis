#pragma once
#include <string>

#include "core/render/util/Color.h"
#include "core/render/util/TextAlign.h"
#include <functional>

class IRenderer {
public:
    IRenderer() = default;

    virtual ~IRenderer() = default;

    virtual int window_width() const = 0;
    virtual int window_height() const = 0;
    virtual bool init(std::string title, int width, int height) = 0;
    virtual bool tint_texture(const std::string &id, const Color& tint = {255,255,255,255}) const = 0;
    virtual bool load_texture(const std::string &id, const std::string &path) = 0;
    virtual bool draw_texture_centered(const std::string &id, int x, int y, int w, int h = -1) const = 0;
    virtual bool draw_texture(const std::string &id, int x, int y, int w, int h = -1) const = 0;
    virtual bool load_font(const std::string &id, const std::string &path, int size) = 0;
    virtual int font_height(const std::string &id) const = 0;
    virtual bool draw_text(const std::string &text, int x, int y, Color color, const std::string &font_id = "default", const TextAlign align = TextAlign::Left) const = 0;
    virtual int text_height(const std::string &text, const std::string &font_id = "default") const = 0;
    virtual int text_width(const std::string &text, const std::string &font_id = "default") const = 0;
    virtual void clear() const = 0;
    virtual void present() = 0;
    virtual void set_draw_color(Color color) const = 0;
    virtual void draw_rect(int x, int y, int w, int h, bool filled = false) const = 0;
    virtual void draw_line(int x1, int y1, int x2, int y2) const = 0;
    virtual void depth_render(double depth, std::function<void(const IRenderer&)> render_func) = 0;
    virtual void destroy() = 0;
};
