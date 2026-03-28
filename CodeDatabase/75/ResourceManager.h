#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager& get();

    const sf::Texture& getTexture(const std::string& name);
    const sf::Font& getFont();

private:
    ResourceManager();
    std::map<std::string, sf::Texture> m_textures;
    sf::Font m_font;
    bool m_fontLoaded;
};