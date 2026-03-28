#pragma once

#include <map>


#include "core/render/renderer/IRenderer.h"
#include "entity/attribute/Attribute.h"
#include "types/EntityType.h"

class SDLWindow;
class World;
class GameLogic;

#include <nlohmann/json.hpp>
using json = nlohmann::json;


inline constexpr double INTERPOLATION_SPEED = 8;


class Entity {
    friend class World;

public:
    Entity(const EntityType type, const int x, const int y) : type_(type), x_(x), y_(y), display_x_(x), display_y_(y) {
    }

    int get_attribute(Attribute attribute) const;

    void set_attribute(Attribute attribute, int value);

    void add_attribute(Attribute attribute, int value);

    void sub_attribute(Attribute attribute, int value);

    virtual void render(const IRenderer &renderer, const World &world, int center_x, int center_y) = 0;

    virtual void render_model(const IRenderer &renderer, const std::string &model, int center_x, int center_y,
                              int offset_y, float width);

    virtual int move_priority() const = 0;

    virtual void update(World &world, float dt);

    virtual void tick(GameLogic &logic);

    void render_health(const IRenderer &renderer, const World &world, int center_x, int center_y) const;

    int x() const {
        return x_;
    }

    int y() const {
        return y_;
    }

    float display_x() const {
        return display_x_;
    }

    float display_y() const {
        return display_y_;
    }

    void reset_display() {
        display_x_ = x_;
        display_y_ = y_;
    }

    EntityType type() const {
        return type_;
    }

    int manh_distance(int x, int y) const;

    int cheb_distance(int x, int y) const;


    int blocked_moves() const {
        return blocked_moves_;
    }

    void block_moves(int moves) {
        blocked_moves_ = std::max(moves, blocked_moves_);
    }

    void skip_blocked_move() {
        if (blocked_moves_ > 0)
            blocked_moves_--;
    }

    virtual bool is_hostile() const {
        return true;
    }

    bool is_ally(const Entity &other) const {
        return other.is_hostile() == is_hostile();
    }

    virtual void on_hurt() {
        hurt_time_ = 0.2;
    }

    virtual json serialize() const {
        json data;

        if (!attributes_.empty()) {
            json attributes = attributes_;
            data["attributes"] = attributes;
        }
        data["blocked_moves"] = blocked_moves_;
        return data;
    }

    virtual bool deserialize(json data) {
        if (!data.contains("blocked_moves")) return false;
        blocked_moves_ = data["blocked_moves"];
        if (data.contains("attributes")) {
            for (auto &pair: data["attributes"]) {
                const auto attr = nlohmann::json(pair[0]).get<Attribute>();
                const auto value = pair[1].get<int>();
                attributes_[attr] = value;
            }
        }


        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const Entity& e) {
        os << "Entity [Type: " << nlohmann::json(e.type_).get<std::string>()
           << ", Pos: (" << e.x() << ", " << e.y() << ")]";
        return os;
    }

    virtual ~Entity() = default;

protected:
    EntityType type_;
    std::map<Attribute, int> attributes_;

    int x_;
    int y_;
    float display_x_ = -1;
    float display_y_ = -1;
    float hurt_time_ = 0;

    int blocked_moves_ = 0;

private:
    void set_x(const int x) {
        if (display_x_ == -1)
            display_x_ = x;
        x_ = x;
    }

    void set_y(const int y) {
        if (display_y_ == -1)
            display_y_ = y;
        y_ = y;
    }
};
