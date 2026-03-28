#pragma once
#include <string>

class SDLWindow;
class Entity;
class World;
class Player;
class GameLogic;
class IRenderer;

class Spell {
public:
    virtual ~Spell();

    virtual std::string id() = 0;

    virtual bool cast(GameLogic &logic, Entity &caster, int mouse_x, int mouse_y, int level) = 0;

    virtual void render_preview(IRenderer &renderer, const World &world, const Player &player, int mouse_x,
                                int mouse_y, int level) = 0;
};
