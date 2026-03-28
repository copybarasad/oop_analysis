#pragma once
#include <memory>

class Entity;

class Cell {
    bool walkable_{true};
    int trapDamage_{0};
    std::shared_ptr<Entity> entity_;
public:
    bool isWalkable() const noexcept { return walkable_; }
    void setWalkable(bool v) noexcept { walkable_ = v; }

    bool hasTrap() const noexcept { return trapDamage_ > 0; }
    void placeTrap(int dmg) noexcept { trapDamage_ = dmg; }
    int triggerTrap() noexcept { int d = trapDamage_; trapDamage_ = 0; return d; }

    bool hasEntity() const noexcept { return !!entity_; }
    std::shared_ptr<Entity> getEntity() const noexcept { return entity_; }
    void setEntity(std::shared_ptr<Entity> e) noexcept { entity_ = std::move(e); }
    void Clear() noexcept {
        walkable_ = true;
        trapDamage_ = 0;
        entity_.reset();
    }
    int trapDamage() const noexcept { return trapDamage_; }

};