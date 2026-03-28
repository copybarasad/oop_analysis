#ifndef UNTITLED_HAND_H
#define UNTITLED_HAND_H

#include <vector>
#include <string>
#include <memory>
#include <optional>
#include "Coord.h"
#include "SpellFactory.h"

class Field;
class Player;

class Hand {
public:
    explicit Hand(size_t capacity = 5);
    bool addSpell(const std::string& spellName);
    bool cast(size_t index, Field& field, Coord casterPos, std::optional<Coord> target, std::shared_ptr<Player> caster);

    const std::vector<std::string>& spells() const noexcept { return spells_; }
    size_t capacity() const noexcept { return capacity_; }
    size_t size() const noexcept { return spells_.size(); }

    void accumulateUpgrade(const UpgradeData& data);
    void clearPendingUpgrade();
    UpgradeData pendingUpgrade() const noexcept { return pending_; }
    bool hasPendingUpgrade() const noexcept { return !pending_.empty(); }

    void setApplyNextUpgrade(bool v) noexcept { applyNext_ = v; }
    bool shouldApplyNextUpgrade() const noexcept { return applyNext_; }

private:
    size_t capacity_;
    std::vector<std::string> spells_;
    UpgradeData pending_;
    bool applyNext_ = false;
    bool enhanceNext = false;
};

#endif // UNTITLED_HAND_H
