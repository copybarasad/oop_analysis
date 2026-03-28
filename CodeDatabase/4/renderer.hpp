#pragma once
#include <memory>

class Field;
class Hero;

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render(const Field& field, const Hero& hero) const = 0;
};