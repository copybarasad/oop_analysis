#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include "OrdinaryCell.hpp"

OrdinaryCell::OrdinaryCell() : Cell(getRandomGrassTexture(), ordinary) {}

std::string OrdinaryCell::getRandomGrassTexture() {
    static const std::vector<std::string> grasses = {
        "./sprites/grass1.png", "./sprites/grass2.png",
        "./sprites/grass3.png", "./sprites/grass4.png", "./sprites/grass5.png"
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, grasses.size() - 1);
    
    return grasses[dist(gen)];
}