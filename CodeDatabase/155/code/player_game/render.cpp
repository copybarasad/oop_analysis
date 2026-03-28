#include <iostream>
#include "render.h"
#include "map.h"

console_renderer::console_renderer(){}

void console_renderer::render(map* map)
{
    map->render();
}

console_renderer::~console_renderer(){}
