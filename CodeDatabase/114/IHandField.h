#ifndef IHANDFIELD_H
#define IHANDFIELD_H

#include <optional>
#include <functional>
#include "ISpellField.h"
#include "Entity.h"

class IHandField : public virtual ISpellField {
public:
    virtual ~IHandField() = default;

    virtual std::optional<std::reference_wrapper<Entity>> findEntityAt(int x, int y) = 0;
};

#endif

