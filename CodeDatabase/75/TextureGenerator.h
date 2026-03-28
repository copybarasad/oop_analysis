#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

class TextureGenerator {
public:
    static void loadAllTextures(std::map<std::string, sf::Texture>& textures);

private:
    static void createTexture(std::map<std::string, sf::Texture>& textures, const std::string& name, int width, int height, const std::vector<std::string>& pattern, const std::map<char, sf::Color>& palette);
};
