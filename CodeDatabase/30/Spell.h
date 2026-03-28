#pragma once
#include <string>
#include <utility>
#include <cstddef>
#include <optional>
#include "../../gamecore/include/Enums.h"
#include "../../gamecore/include/Position.h"
#include "../../gamecore/include/UpgradeState.h"

class Manager;

enum class SpellType {
    DirectDamage,
    AreaDamage,
    Trap,
    Summon,
    Buff
};

class Spell {
public:
    Spell(std::string name, SpellType type, int charges = 1)
        : name_(std::move(name)), type_(type), charges_(charges) {}
    virtual ~Spell() = default;

    const std::string& name() const noexcept { return name_; }
    SpellType type() const noexcept { return type_; }
    int charges() const noexcept { return charges_; }
    bool hasCharges() const noexcept { return charges_ > 0; }
    void addCharges(int c) { charges_ += (c > 0 ? c : 0); }

    // canCast — просто проверка условий (без побочек)
    virtual bool canCast(const Manager& manager, EntityId casterId, const Position& target,
                         const UpgradeState* up = nullptr) const = 0;
    // cast — применяем, при успехе тратим 1 заряд
    virtual bool cast(Manager& manager, EntityId casterId, const Position& target,
                      const UpgradeState* up = nullptr) = 0;

    Spell(const Spell&) = delete;
    Spell& operator=(const Spell&) = delete;
    Spell(Spell&&) = default;
    Spell& operator=(Spell&&) = default;

    virtual void save(std::ostream& os) const;
    virtual void load(std::istream& is);

protected:
    bool consumeOne() { if (charges_ <= 0) return false; --charges_; return true; }
    
    std::optional<Position> getCasterPosIfValid(const Manager& manager, EntityId casterId) const;

    std::string name_;
    SpellType type_;
    int charges_{};
};
