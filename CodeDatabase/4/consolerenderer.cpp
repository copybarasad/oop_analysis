#include "consolerenderer.hpp"
#include "field.hpp"
#include "hero.hpp"
#include <iostream>

void ConsoleRenderer::render(const Field& field, const Hero& hero) const {
    field.show(hero);
}