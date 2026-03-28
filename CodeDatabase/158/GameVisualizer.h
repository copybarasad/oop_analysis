#pragma once

#include "Input/KeyBinder.h"
#include "Core/Map.h"
#include "Entities/Player.h"

template<typename Renderer>
class GameVisualizer 
{
private:
    Renderer renderer;
    
public:
    void drawMap(const Map& map) 
    {
        renderer.render(map);
    }
    
    void drawUI(const Player& player, int level, const std::vector<std::pair<int, int>>& spells) 
    {
        renderer.renderUI(player, level, spells);
    }
    
    void drawControls(const KeyBinder& binder) 
    {
        renderer.renderControls(binder);
    }
    
    void drawMessage(const std::string& message) 
    {
        renderer.renderMessage(message);
    }
};