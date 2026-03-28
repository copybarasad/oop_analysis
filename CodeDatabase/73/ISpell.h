#ifndef GAMELB1_ISPELL_H
#define GAMELB1_ISPELL_H

#include <string>
#include <memory>

class Game;

class ISpell {
public:
    virtual ~ISpell() = default;

    virtual void apply(Game& game, int target_x, int target_y) = 0;

    virtual std::unique_ptr<ISpell> clone() const = 0;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    virtual int getTypeId() const = 0;
};

#endif //GAMELB1_ISPELL_H