#pragma once

#include <string>
#include <functional>

#include "core/render/renderer/IRenderer.h"
#include "core/render/util/TextAlign.h"


enum class TextAlign;

class UIButton {
private:
    std::string texture_;
    std::string text_;
    int text_y_padding_ = 0;
    int text_x_padding_ = 0;
    TextAlign text_align_ = TextAlign::Center;
    std::string text_font_ = "default";

    int min_x_;
    int min_y_;
    int width_;
    int height_;

    std::function<void()> on_click_;
    std::function<void(const IRenderer&)> custom_render_;

public:
    UIButton(const std::string& texture, const int x, const int y, const int w, const int h): texture_(texture), min_x_(x), min_y_(y), width_(w), height_(h) {}

    void set_on_click(const std::function<void()> &callback) {
        on_click_ = callback;
    }

    void set_custom_render(const std::function<void(const IRenderer&)> &callback) {
        custom_render_ = callback;
    }

    void set_text(const std::string& text) {
        text_ = text;
    }

    void set_text_align(const TextAlign align) {
        text_align_ = align;
    }

    void set_text_font(const std::string& font) {
        text_font_ = font;
    }

    void set_text_x_padding(const int padding) {
        text_x_padding_ = padding;
    }

    void set_text_y_padding(const int padding) {
        text_y_padding_ = padding;
    }


    bool is_inside(const int x, const int y) const {
        return (x >= min_x_ && x <= min_x_ + width_ && y >= min_y_ && y <= min_y_ + height_);
    }

    bool handle_click(const int x, const int y) const{
        if (is_inside(x, y) && on_click_) {
            on_click_();
            return true;
        }
        return false;
    }

    void render(const IRenderer& renderer) const {
        renderer.draw_texture(texture_, min_x_, min_y_, width_, height_);
        if (!text_.empty()) {
            int text_x = min_x_;
            if (text_align_ == TextAlign::Center) {
                text_x += width_ / 2;
            } else if (text_align_ == TextAlign::Right) {
                text_x += width_;
            }

            renderer.draw_text(text_, text_x + text_x_padding_, min_y_ + text_y_padding_, {0,0,0,255}, text_font_, text_align_);
        }
        if(custom_render_){
            custom_render_(renderer);
        }
    }
};
