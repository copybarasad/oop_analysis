#include "UpgradeSystem.h"
#include <iostream>

UpgradeSystem::UpgradeSystem(std::shared_ptr<TextSystem> textSystem) 
    : textSystem(textSystem) {}

void UpgradeSystem::showUpgradeMenu(Player& player) {
    std::cout << textSystem->getUpgradeMenuTitle() << std::endl;
    std::cout << textSystem->getAvailablePoints(player.getScore()) << std::endl;
    std::cout << std::endl;
    
    std::cout << textSystem->getUpgradeOption(1, "Увеличение здоровья (+20 HP)", 15) << std::endl;
    std::cout << textSystem->getUpgradeOption(2, "Увеличение урона (+3 урона)", 20) << std::endl;
    std::cout << textSystem->getUpgradeOption(3, "Усиление заклинаний", 25) << std::endl;
    std::cout << textSystem->getContinueOption(5) << std::endl;
    std::cout << textSystem->getSelectOption();
}

void UpgradeSystem::applyUpgrade(Player& player, UpgradeType upgrade) {
    int cost = getUpgradeCost(upgrade);
    
    if (player.getScore() < cost) {
        std::cout << textSystem->getNotEnoughPointsMessage() << std::endl;
        return;
    }
    
    switch (upgrade) {
        case UpgradeType::HEALTH_BOOST:
            player.setHealth(120);
            player.setScore(player.getScore() - cost);
            std::cout << textSystem->getUpgradeAppliedMessage("Увеличение здоровья") << std::endl;
            std::cout << textSystem->getHealthUpgradeMessage(player.getHealth()) << std::endl;
            break;
            
        case UpgradeType::DAMAGE_BOOST:
            player.setDamage(player.getDamage() + 3);
            player.setScore(player.getScore() - cost);
            std::cout << textSystem->getUpgradeAppliedMessage("Увеличение урона") << std::endl;
            break;
            
        case UpgradeType::SPELL_POWER:
            player.setScore(player.getScore() - cost);
            std::cout << textSystem->getUpgradeAppliedMessage("Усиление заклинаний") << std::endl;
            break;
    }
}

int UpgradeSystem::getUpgradeCost(UpgradeType upgrade) const {
    switch (upgrade) {
        case UpgradeType::HEALTH_BOOST: return 15;
        case UpgradeType::DAMAGE_BOOST: return 20;
        case UpgradeType::SPELL_POWER: return 25;
        default: return 0;
    }
}