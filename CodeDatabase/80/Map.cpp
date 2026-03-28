#include "Map.h"
#include "Entity.h"
#include <iostream>
#include <cstring>
#include <sstream>

const std::string Map::MAP_HEADER = "MAP";

Map::Map(int weight, int height) : weight(weight), height(height) {
    map_arr.resize(height);
    for (int i = 0; i < height; i++) {
        map_arr[i].resize(weight, '0');
    }
    map_arr[1][1] = '*';
    map_arr[2][4] = '*';
    map_arr[2][3] = '/';
    map_arr[3][3] = '/';
}

Map::Map(const Map& other) : weight(other.weight), height(other.height) {
    map_arr.resize(height);
    for (int i = 0; i < height; i++) {
        map_arr[i].resize(weight);
        for (int j = 0; j < weight; j++) {
            map_arr[i][j] = other.map_arr[i][j];
        }
    }
}

Map::Map(Map&& other) noexcept
    : weight(other.weight),
    height(other.height),
    map_arr(std::move(other.map_arr)),
    obj_map(std::move(other.obj_map)) {
    other.weight = 0;
    other.height = 0;
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        weight = other.weight;
        height = other.height;

        map_arr.resize(height);
        for (int i = 0; i < height; i++) {
            map_arr[i] = other.map_arr[i];
        }

        obj_map.clear();
    }
    return *this;
}

Map& Map::operator=(Map&& other) noexcept {
    if (this != &other) {
        weight = other.weight;
        height = other.height;
        map_arr = std::move(other.map_arr);
        obj_map = std::move(other.obj_map);

        other.weight = 0;
        other.height = 0;
    }
    return *this;
}


void Map::SetVal(int x, int y, char val, Entity* obj) {
    if (x >= 0 && x < height && y >= 0 && y < weight) {
        if (val == '0') {
            obj_map.erase({ x, y });
        }
        else {
            obj_map[{x, y}] = obj;
        }
        map_arr[x][y] = val;
    }
}

char Map::GetVal(int x, int y) {
    if (x >= 0 && x < height && y >= 0 && y < weight)
        return map_arr[x][y];
    return -1;
}

void Map::SeeMap() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            std::cout << map_arr[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

bool Map::CanMoveTo(int x, int y) {
    if (x >= 0 && x < height && y >= 0 && y < weight) {
        return (map_arr[x][y] == '0' || map_arr[x][y] == '*');
    }
    return false;
}

std::map<std::pair<int, int>, Entity*>& Map::GetObjects() {
    return obj_map;
}

int Map::GetWeight() const {
    return weight;
}

int Map::GetHeight() const {
    return height;
}

void Map::CheckTraps(int x, int y, Entity* entity) {
    if (map_arr[x][y] == '*') {
        if (entity->ind_type == 'e') {
            std::cout << "Trap activated! Enemy takes damage and is slowed!" << std::endl;
            entity->TakeDamage(25, *this);
            entity->Slowed();
            map_arr[x][y] = '0';
        }
        else if (entity->ind_type == 'H') {
            std::cout << "Player stepped on a trap! Player is slowed!" << std::endl;
            entity->Slowed();
            map_arr[x][y] = '0';
        }
    }
}

std::vector<uint8_t> Map::Save() const {
    size_t dataSize = MAP_HEADER_SIZE + (weight * height * sizeof(char));
    std::vector<uint8_t> data(dataSize);
    size_t offset = 0;

    memcpy(data.data() + offset, &weight, sizeof(weight));
    offset += sizeof(weight);

    memcpy(data.data() + offset, &height, sizeof(height));
    offset += sizeof(height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            char cell = map_arr[i][j];
            memcpy(data.data() + offset, &cell, sizeof(cell));
            offset += sizeof(cell);
        }
    }

    return data;
}

void Map::Load(const std::vector<uint8_t>& data) {
    if (data.size() < MAP_HEADER_SIZE) return;

    size_t offset = 0;

    int new_weight, new_height;
    memcpy(&new_weight, data.data() + offset, sizeof(new_weight));
    offset += sizeof(new_weight);

    memcpy(&new_height, data.data() + offset, sizeof(new_height));
    offset += sizeof(new_height);

    size_t expectedSize = MAP_HEADER_SIZE + (new_weight * new_height * sizeof(char));
    if (data.size() != expectedSize) return;

    map_arr.resize(new_height);
    for (int i = 0; i < new_height; i++) {
        map_arr[i].resize(new_weight);
        for (int j = 0; j < new_weight; j++) {
            char cell;
            memcpy(&cell, data.data() + offset, sizeof(cell));
            offset += sizeof(cell);
            map_arr[i][j] = cell;
        }
    }

    weight = new_weight;
    height = new_height;
}

size_t Map::GetMapSaveSize() const {
    return MAP_HEADER_SIZE + (weight * height * sizeof(char));
}

void Map::ClearAllObjects() {
    for (auto it = obj_map.begin(); it != obj_map.end(); ) {
        if (it->second && it->second->ind_type == 'e') {
            delete it->second;
            it = obj_map.erase(it);
        }
        else {
            ++it;
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            if (map_arr[i][j] == 'e') {
                map_arr[i][j] = '0';
            }
        }
    }
}

std::string Map::SaveToString() const {
    std::ostringstream ss;
    ss << "width:" << weight << "\n";
    ss << "height:" << height << "\n";
    ss << "data:\n";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < weight; j++) {
            ss << map_arr[i][j];
        }
        ss << "\n";
    }

    return ss.str();
}



void Map::LoadFromString(const std::string& data) {
    std::istringstream ss(data);
    std::string line;
    bool readingData = false;
    int currentRow = 0;

    map_arr.clear();
    weight = 0;
    height = 0;

    while (std::getline(ss, line)) {
        if (line == "data:") {
            readingData = true;
            continue;
        }

        if (!readingData) {
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) continue;

            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            if (key == "width") weight = std::stoi(value);
            else if (key == "height") height = std::stoi(value);
        }
        else {
            if (map_arr.empty() && height > 0 && weight > 0) {
                map_arr.resize(height);
                for (int i = 0; i < height; i++) {
                    map_arr[i].resize(weight, '0');
                }
            }

            if (currentRow < height && line.length() == static_cast<size_t>(weight)) {
                for (int j = 0; j < weight; j++) {
                    map_arr[currentRow][j] = line[j];
                }
                currentRow++;
            }
        }
    }

    if (map_arr.empty()) {
        std::cout << "Warning: Map data loading failed, using default map" << std::endl;
        weight = 20;
        height = 20;
        map_arr.resize(height);
        for (int i = 0; i < height; i++) {
            map_arr[i].resize(weight, '0');
        }
    }
}

std::string Map::GetMapHeader() const {
    return MAP_HEADER;
}