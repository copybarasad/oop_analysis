#include "WorldTile.h"


bool WorldTile::can_go() const {
    return type_ != TileType::Lava && !occupied();
}

TileType WorldTile::type() const {
    return type_;
}

bool WorldTile::set_type(const TileType type) {
    if (occupied() && !can_go()) {
        return false;
    }
    type_ = type;
    return true;
}



bool WorldTile::occupied() const {
    return entity_ != nullptr;
}
