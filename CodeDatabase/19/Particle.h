#pragma once
#include <algorithm>
#include <string>
#include <utility>

#include "../util/Color.h"

class Particle {
public:
    Particle(const std::string& texture, const float ttl, const bool world_relative, const float size, const Color &color_a, const Color &color_b,
        const std::pair<float, float> &position, const std::pair<float, float> &velocity,
        const std::pair<float, float> &acceleration, const std::pair<float, float> &drag)
        : texture_(texture),
          TTL_(ttl),
          world_relative_(world_relative),
          size_(size),
          colorA_(color_a),
          colorB_(color_b),
          position_(position),
          velocity_(velocity),
          acceleration_(acceleration),
          drag_(drag),
          current_color_(color_a),
          seconds_lived_(0) {
    }

    void tick(const float dt) {
        seconds_lived_ += dt;

        velocity_.first += acceleration_.first * dt;
        velocity_.second += acceleration_.second * dt;

        velocity_.first *= (1.0f - drag_.first * dt);
        velocity_.second *= (1.0f - drag_.second * dt);

        position_.first += velocity_.first * dt;
        position_.second += velocity_.second * dt;


        const float factor = std::min(1.0f, seconds_lived_ / TTL_);

        current_color_.r = static_cast<uint8_t>(colorA_.r + (colorB_.r - colorA_.r) * factor);
        current_color_.g = static_cast<uint8_t>(colorA_.g + (colorB_.g - colorA_.g) * factor);
        current_color_.b = static_cast<uint8_t>(colorA_.b + (colorB_.b - colorA_.b) * factor);
        current_color_.a = static_cast<uint8_t>(colorA_.a + (colorB_.a - colorA_.a) * factor);
    }

    bool dead() const {
        return seconds_lived_ >= TTL_;
    }

    [[nodiscard]] std::string texture() const {
        return texture_;
    }

    [[nodiscard]] float ttl() const {
        return TTL_;
    }

    [[nodiscard]] bool world_relative() const {
        return world_relative_;
    }

    [[nodiscard]] float size() const {
        return size_;
    }


    [[nodiscard]] float seconds_lived() const {
        return seconds_lived_;
    }

    [[nodiscard]] Color color_a() const {
        return colorA_;
    }

    [[nodiscard]] Color color_b() const {
        return colorB_;
    }

    [[nodiscard]] Color current_color() const {
        return current_color_;
    }

    [[nodiscard]] std::pair<float, float> position() const {
        return position_;
    }

    [[nodiscard]] std::pair<float, float> velocity() const {
        return velocity_;
    }

    [[nodiscard]] std::pair<float, float> acceleration() const {
        return acceleration_;
    }

    [[nodiscard]] std::pair<float, float> drag() const {
        return drag_;
    }

    void set_texture(const std::string &texture) {
        texture_ = texture;
    }

    void set_velocity(const std::pair<float, float> &velocity) {
        velocity_ = velocity;
    }

    void set_acceleration(const std::pair<float, float> &acceleration) {
        acceleration_ = acceleration;
    }

    void set_drag(const std::pair<float, float> &drag) {
        drag_ = drag;
    }

private:
    std::string texture_;
    float TTL_;
    bool world_relative_;
    float size_;
    Color colorA_;
    Color colorB_;
    std::pair<float, float> position_;
    std::pair<float, float> velocity_;
    std::pair<float, float> acceleration_;
    std::pair<float, float> drag_;
    Color current_color_;
    float seconds_lived_;
};
