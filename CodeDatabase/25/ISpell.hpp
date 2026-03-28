#pragma once

#include <memory>
#include <optional>
#include <string>

#include "Position.hpp"

class Field;
class Player;
class ISpell {
public:
    virtual ~ISpell() = default;
    virtual std::string name() const = 0;
    virtual int cost() const = 0;
    virtual std::unique_ptr<ISpell> clone() const = 0;
    virtual bool canCast(const Field& f, const Player& p) const = 0;
    virtual bool cast(Field& f, Player& p, std::optional<Pos> target) = 0;
};
