#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <deque>
#include <vector>

struct VisualEvent {
    enum Type { 
        LIGHTNING, 
        EXPLOSION, 
        NOVA, 
        BEAM,
        FIRE_STORM, 
        ICE_BLAST,
        SLASH,
        BLOOD
    };
    Type type;
    float startX, startY;
    float endX, endY;
    float lifetime;
    float maxLifetime;
    sf::Color color;
};

class VisualEffects {
public:
    struct LogMessage {
        std::string text;
        sf::Color color;
        float lifetime;
    };

    static void initialize() {} 
    
    static void printColored(const std::string& text, sf::Color color = sf::Color::White);
    static void updateLogs(float dt);
    static const std::deque<LogMessage>& getLogs();

    enum class Color { RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE, DEFAULT };
    static void printColored(const std::string& text, Color color);
    
    static void clearScreen() {}
    static void printTitle() {}
    static void setCursorPosition(int, int) {}
    static std::string getGameSymbol(const std::string&) { return ""; }

private:
    static std::deque<LogMessage> m_logs;
};