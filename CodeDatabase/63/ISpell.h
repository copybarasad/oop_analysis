#ifndef RPG_ISPELL_H
#define RPG_ISPELL_H

#include <memory>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
class EntityManager;
class GameField;
class Entity;

class ISpell {
public:
    ISpell() = default;

    virtual ~ISpell() = default;

    [[nodiscard]] virtual std::shared_ptr<ISpell> clone() const = 0;

    [[nodiscard]] virtual json to_json() const = 0;

    [[nodiscard]] virtual std::string getName() const = 0;

    virtual bool cast(std::pair<int,int>& coordinates, Entity* caster, GameField& field, EntityManager& entityManager) = 0;

    [[nodiscard]] virtual int getLevel() const = 0;

    virtual void setLevel(int newLevel) = 0;

    [[nodiscard]] virtual std::pair<int, int> visualData() const = 0;

    [[nodiscard]] virtual bool isReady() const = 0;

    virtual bool reload() = 0;

    virtual void fullReload() = 0;

    [[nodiscard]] virtual int getReloadLeft() const = 0;

    [[nodiscard]] virtual int getCastDiameter() const = 0;

    [[nodiscard]] virtual bool allCellsInRange(std::pair<int,int>& target) const = 0;
};

#endif
