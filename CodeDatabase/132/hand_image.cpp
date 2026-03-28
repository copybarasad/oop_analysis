#include "hand_image.hpp"

void HandImage::remove_half() {
    if (cards.empty()) return;
    
    std::string shuffled = cards;
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), g);
    
    cards = shuffled.substr(0, (shuffled.size() + 1) / 2);
}