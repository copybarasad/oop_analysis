//
// Created by Artem on 26.12.2025.
//

#ifndef LABAOOP2_GAMEVISUALIZER_H
#define LABAOOP2_GAMEVISUALIZER_H


#include <vector>
#include <memory>
#include "../Entities/Entity.h"
#include "../GameField/Field.h"
#include "../Entities/Creatures/PlayerView.h"

template<typename DrawStrategy>
class GameVisualizer {
public:
    GameVisualizer(const Field &field,
                   const std::vector<std::shared_ptr<Entity>> &entities,
                   PlayerView &player)
            : drawStrategy(field, entities, player)
    {}

    void update() {
        drawStrategy.draw();
    }

    void prepareDisplay() {
        drawStrategy.prepareConsole();
    }

    void clear() {
        drawStrategy.clearDisplay();
    }

private:
    DrawStrategy drawStrategy;
};

#endif //LABAOOP2_GAMEVISUALIZER_H
