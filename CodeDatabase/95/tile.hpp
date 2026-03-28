#ifndef TILE_HPP
#define TILE_HPP

enum class TileType {
    Grass,
    Wall,
    Trap
};

class Tile {
protected:
    TileType type;
    bool walkable;

public:
    Tile(TileType type_ = TileType::Grass);

    bool isWalkable() const;
    TileType getType() const;

};

#endif