#ifndef TEMPLATE_RENDERER_HPP
#define TEMPLATE_RENDERER_HPP

#include "game/entity_manager.hpp"
#include <memory>

template <typename RendererImpl>
class Renderer {
private:
    RendererImpl renderer;
    
public:
    Renderer() = default;
    
    void render(eManager& manager) {
        renderer.draw(manager);
    }
    
    RendererImpl& getRenderer() { return renderer; }
    const RendererImpl& getRenderer() const { return renderer; }

};

#endif