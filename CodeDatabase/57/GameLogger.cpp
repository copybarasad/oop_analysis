#include "GameLogger.hpp"



GameLogger::GameLogger() {
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf")) {
        std::cout << "ШРИФТ НЕ ЗАГРУЗИЛСЯ" << std::endl;
        return;
    }
    
    texture.loadFromFile("./sprites/switoc.png");
    sprite.setTexture(texture);
    sprite.setPosition(LOCATION_SWITOC.x, LOCATION_SWITOC.y);

    text.setFont(font);
    text.setCharacterSize(SIZE_TEXT);
    text.setFillColor(COLOR_TEXT);
    text.setPosition(LOCATION_TEXT.x, LOCATION_TEXT.y);
     
    log("The console is initialized!\n");
}


void GameLogger::log(const std::string& message) {
    messages.push_back(message);
    if (messages.size() > 40) {
        messages.erase(messages.begin());
    }
    
    std::string displayText; 
    for (std::string& msg : messages) {
        displayText += msg;
    }
    text.setString(displayText);
} 


void GameLogger::draw(sf::RenderWindow& window) { 
    
    window.draw(sprite);
    window.draw(text); 
}