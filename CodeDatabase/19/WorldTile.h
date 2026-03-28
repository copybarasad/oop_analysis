#pragma once

#include "TileType.h"
#include "entity/Entity.h"


class WorldTile {
    friend class World;

public:
    explicit WorldTile(const TileType type) : type_(type) {
    }

    WorldTile() : type_(TileType::Ground) {
    }

    bool can_go() const;

    bool occupied() const;

    TileType type() const;

    bool set_type(TileType type);

    Entity *entity() const {
        return entity_;
    }

private:
    TileType type_;
    Entity *entity_ = nullptr;
};
