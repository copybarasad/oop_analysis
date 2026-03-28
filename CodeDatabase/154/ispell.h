#ifndef ISPELL_H
#define ISPELL_H

#include <string>

class Field;

class ISpell {
public:
    virtual std::string getName() const = 0;
    virtual ~ISpell() = default;
    virtual bool apply(int casterId, Field& field, int targetRow, int targetCol) = 0;
};

#endif
