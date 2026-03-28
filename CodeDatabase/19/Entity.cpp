#include "Entity.h"

#include "world/World.h"


void Entity::update(World &world, float dt) {
    float dx = static_cast<float>(x_) - display_x_;
    float dy = static_cast<float>(y_) - display_y_;
    float dist = std::sqrt(dx * dx + dy * dy);

    if (dist > 0.001f) {
        const float step = INTERPOLATION_SPEED * dt;
        if (step >= dist) {

            display_x_ = static_cast<float>(x_);
            display_y_ = static_cast<float>(y_);
        } else {
            display_x_ += dx / dist * step;
            display_y_ += dy / dist * step;
        }
    }
    hurt_time_-=dt;
    if (hurt_time_ < 0) {
        hurt_time_ = 0.0f;
    }

}

void Entity::tick(GameLogic &logic) {
}

int Entity::get_attribute(const Attribute attribute) const {
    const auto it = attributes_.find(attribute);
    if (it != attributes_.end()) {
        return it->second;
    }
    return 0.0;
}

void Entity::set_attribute(const Attribute attribute, const int value) {
    attributes_[attribute] = std::max(value, 0);
    if (attribute == Attribute::MaxHealth) {
        int health = get_attribute(Attribute::Health);
        if (health > value) {
            set_attribute(Attribute::Health, value);
        }
    }
}

void Entity::add_attribute(const Attribute attribute, const int value) {
    set_attribute(attribute, get_attribute(attribute) + value);
}

void Entity::sub_attribute(const Attribute attribute, const int value) {
    set_attribute(attribute, get_attribute(attribute) - value);
}

void Entity::render_model(const IRenderer &renderer, const std::string &model, const int center_x, const int center_y, const int offset_y, const float width) {
    if (hurt_time_ > 0) {
        (void)renderer.tint_texture(model, {255, 100,100, 255});
    }else {
        (void)renderer.tint_texture(model);
    }

    (void)renderer.draw_texture_centered(model, center_x, center_y - GUI_SCALE * offset_y, GUI_SCALE * width);
}

void Entity::render_health(const IRenderer &renderer, const World &world, int center_x, int center_y) const {
    int max = get_attribute(Attribute::MaxHealth);
    int current = get_attribute(Attribute::Health);
    if (max <= 0) return;


    int bar_width = GUI_SCALE * 15;
    int bar_height = GUI_SCALE * 3;

    int render_x = center_x - bar_width / 2;
    int render_y = center_y - bar_height / 2;

    renderer.set_draw_color({255, 0, 0, 255});


    int filled_width = static_cast<float>(current) / max * bar_width;

    renderer.draw_rect(render_x, render_y, filled_width, bar_height, true);
    renderer.set_draw_color({0, 0, 0, 255});
    renderer.draw_rect(render_x, render_y, bar_width, bar_height);
}

int Entity::manh_distance(int x, int y) const {
    return abs(x - x_) + abs(y - y_);
}

int Entity::cheb_distance(int x, int y) const {
    return std::max(abs(x - x_), abs(y - y_));
}


