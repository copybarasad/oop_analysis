#include "VisualEffects.h"

std::deque<VisualEffects::LogMessage> VisualEffects::m_logs;

void VisualEffects::printColored(const std::string& text, sf::Color color) {
    m_logs.push_back({text, color, 4.0f}); // Сообщение висит 4 секунды
    if (m_logs.size() > 8) m_logs.pop_front();
}

void VisualEffects::printColored(const std::string& text, Color color) {
    sf::Color c = sf::Color::White;
    switch(color) {
        case Color::RED: c = sf::Color::Red; break;
        case Color::GREEN: c = sf::Color::Green; break;
        case Color::YELLOW: c = sf::Color::Yellow; break;
        case Color::BLUE: c = sf::Color::Cyan; break;
        case Color::PURPLE: c = sf::Color::Magenta; break;
        default: break;
    }
    printColored(text, c);
}

void VisualEffects::updateLogs(float dt) {
    for (auto& log : m_logs) log.lifetime -= dt;
    while (!m_logs.empty() && m_logs.front().lifetime <= 0) m_logs.pop_front();
}

const std::deque<VisualEffects::LogMessage>& VisualEffects::getLogs() {
    return m_logs;
}