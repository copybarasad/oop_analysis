#pragma once

#include "objects/GameObject.h"
#include "objects/Player.h"
#include "objects/Enemy.h"
#include <vector>
#include <memory>

class Field {
    int width;
    int height;
    Player* player;
    std::vector<std::unique_ptr<GameObject>> objects;

public:
    Field(int width, int height)
        : width(width), height(height), player(nullptr) {}

    // Deep copy
    Field(const Field& other)
        : width(other.width), height(other.height), player(other.player) {
        objects.reserve(other.objects.size());
        for (const auto& obj : other.objects) {
            objects.push_back(obj->clone());
        }
    }

    // Move
    Field(Field&& other) noexcept
        : width(other.width), height(other.height), 
          player(other.player), objects(std::move(other.objects)) {
        other.width = 0;
        other.height = 0;
        other.player = nullptr;
    }

    // Copy = 
    Field& operator=(const Field& other) {
        if (this != &other) {
            width = other.width;
            height = other.height;
            player = other.player;
            
            objects.clear();
            objects.reserve(other.objects.size());
            for (const auto& obj : other.objects) {
                objects.push_back(obj->clone());
            }
        }
        return *this;
    }

    // Move =
    Field& operator=(Field&& other) noexcept {
        if (this != &other) {
            width = other.width;
            height = other.height;
            player = other.player;
            objects = std::move(other.objects);
            
            other.width = 0;
            other.height = 0;
            other.player = nullptr;
        }
        return *this;
    }

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Player* getPlayer() { return player; }
    const std::vector<std::unique_ptr<GameObject>>& getObjects() const {
        return objects;
    }

    // Setters
    void setPlayer(Player* p) { player = p; }

    void addObject(std::unique_ptr<GameObject> obj) {
        objects.push_back(std::move(obj));
    }

    bool isInBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    std::vector<GameObject*> getObjectsAt(int x, int y) {
        std::vector<GameObject*> result;
        for (auto& obj : objects) {
            if (obj->getX() == x && obj->getY() == y) {
                result.push_back(obj.get());
            }
        }
        return result;
    }

    GameObject* getObjectAt(int x, int y);
    void removeDeadEnemies();
    bool hasEnemies() const;
};
