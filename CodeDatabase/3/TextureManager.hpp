#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include "ViewController/Managers/paths.hpp"
#include "ViewController/Managers/render_types.hpp"
#include "types.hpp"

class TextureManager {
public:
    TextureManager();

    sf::Texture& getTexture(RenderFieldType t);

private:
    std::map<RenderFieldType, std::unique_ptr<sf::Texture>> fieldTextures;
};
