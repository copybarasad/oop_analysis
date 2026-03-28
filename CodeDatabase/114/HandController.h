#ifndef HANDCONTROLLER_H
#define HANDCONTROLLER_H

#include "interfaces/IHandField.h"
#include "spells/ISpellFactory.h"
#include <optional>
#include <cstddef>

class HandController {
private:
    IHandField* field;
    ISpellFactory* spellFactory;
public:
    explicit HandController(IHandField* field, ISpellFactory* spellFactory) 
        : field(field), spellFactory(spellFactory) {}

    bool castSpellFromHandIndex(std::size_t index, std::optional<int> targetX = std::nullopt, std::optional<int> targetY = std::nullopt);
};

#endif
