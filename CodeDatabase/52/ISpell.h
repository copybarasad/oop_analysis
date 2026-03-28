#pragma once
#include <string>
#include <optional>
#include <utility>

class Player;
class GameField;

struct SpellContext {
    std::optional<std::pair<int,int>> targetCell;
};

struct ISpell {
    virtual ~ISpell() = default;
    virtual std::string Name() const noexcept = 0;
    virtual int ManaCost() const noexcept = 0;
    virtual bool Cast(Player& player, GameField& field, const SpellContext& ctx) = 0;
};