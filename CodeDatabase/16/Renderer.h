#ifndef RENDERER_H
#define RENDERER_H

#include "Field.h"
#include "EntityManager.h"
#include "GameTypes.h" 

class Renderer {
public:
    void render(const Field& field, const EntityManager& entity_manager, GameState game_state, const Point& cursor) const;

private:
    void renderField(const Field& field, const EntityManager& entity_manager, GameState game_state, const Point& cursor) const;
    void renderUI(const EntityManager& entity_manager, GameState game_state, const Point& cursor) const;
};

#endif