#include "GameObjects/Map.hpp"

Map::Map(MapDTO map) {
    fields.resize(map.height);
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            fields[y].push_back(Field(Coordinates{x, y}, FieldType::FIELD_DEFAULT));
        }
    }

    for (const auto& f : map.FPs) {
        fields[f.coord.y][f.coord.x] = Field(f.coord, f.type);
    }
}


void Map::putMapSave(MapDTO map) {
    for (const auto& f : map.FPs) {
        fields[f.coord.y][f.coord.x].setStatus(f.type);
    }
}


Map::Map(const Map& other) {
    this->fields = other.fields;
}


Map::Map(Map&& other) {
    this->fields = other.fields;
}


Map& Map::operator=(const Map& other) {
    if (this == &other) return *this;
    this->fields = other.fields;

    return *this;
}


Map& Map::operator=(Map&& other) {
    if (this == &other) return *this;
    this->fields = other.fields;

    return *this;
}


Field& Map::getField(int x, int y) {
    return this->fields[y][x];
}

Field& Map::getField(Coordinates coord) {
    return this->fields[coord.y][coord.x];
}
