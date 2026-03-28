#include "ResourceManager.h"
#include "TextureGenerator.h"
#include <iostream>

ResourceManager& ResourceManager::get() {
    static ResourceManager instance;
    return instance;
}

const sf::Texture& ResourceManager::getTexture(const std::string& name) {
    if (m_textures.find(name) == m_textures.end()) {
        if (name.find(".png") != std::string::npos) {
            sf::Texture tex;
            // Try load or create placeholder
            if (!tex.loadFromFile(name)) {
                sf::Image img; 
                img.resize({32,32}, sf::Color::Magenta);
                tex.loadFromImage(img);
            }
            m_textures[name] = tex;
        } else {
            // Return empty/magenta if procedural gen failed
            static sf::Texture empty;
            if (empty.getSize().x == 0) {
                sf::Image img; img.resize({32,32}, sf::Color::Magenta);
                empty.loadFromImage(img);
            }
            return empty;
        }
    }
    return m_textures.at(name);
}

const sf::Font& ResourceManager::getFont() {
    if (!m_fontLoaded) {
        // Try common system fonts
        const char* fonts[] = {
            "/System/Library/Fonts/Helvetica.ttc",
            "C:\\Windows\\Fonts\\arial.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "arial.ttf"
        };
        
        for (const char* f : fonts) {
            if (m_font.openFromFile(f)) {
                m_fontLoaded = true;
                break;
            }
        }
        
        if (!m_fontLoaded) {
            // Fallback: This might crash if no font found, but better than nothing
            std::cerr << "ERROR: No font found!" << std::endl;
        }
    }
    return m_font;
}

ResourceManager::ResourceManager() : m_fontLoaded(false) {
    // Generate Procedural Textures on Startup
    TextureGenerator::loadAllTextures(m_textures);
    
    // Map old names to new gen textures for compatibility if needed
    if (m_textures.count("player")) m_textures["player.png"] = m_textures["player"];
    if (m_textures.count("orc")) m_textures["enemy.png"] = m_textures["orc"];
    if (m_textures.count("wall_tex")) m_textures["wall.png"] = m_textures["wall_tex"];
    if (m_textures.count("floor_tex")) m_textures["floor.png"] = m_textures["floor_tex"];
    
    // Ensure obstacle.png exists (mapped to rock usually)
    if (m_textures.count("rock")) m_textures["obstacle.png"] = m_textures["rock"];
}