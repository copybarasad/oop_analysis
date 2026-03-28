#pragma once
#include <string>
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "gamemodel.hpp"
#include "spell_defaults.hpp" 

template <typename Renderer>
class Visualizer {
public:
    Visualizer(Game& game)
        : game_(game),
          renderer_(std::make_shared<Renderer>(game)) 
    {}

private:
    Game& game_;
    std::shared_ptr<Renderer> renderer_;
};
