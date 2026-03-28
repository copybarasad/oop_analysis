#include "GameRenderer.h"
#include <iostream>
#include <limits>

void GameRenderer::waitForAnyKey() {
    std::cout << "\nPress any key to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string GameRenderer::centerText(const std::string& text, int width) {
    if (text.length() >= static_cast<size_t>(width)) {
        return text;
    }
    int leftPadding = (width - static_cast<int>(text.length())) / 2;
    int rightPadding = width - static_cast<int>(text.length()) - leftPadding;
    return std::string(leftPadding, ' ') + text + std::string(rightPadding, ' ');
}

std::string GameRenderer::repeatChar(char ch, int count) {
    return std::string(count, ch);
}
