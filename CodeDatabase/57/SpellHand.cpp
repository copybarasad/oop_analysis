#include "SpellHand.hpp"


void SpellHand::addRandomSpell() {
    if (unindicted_spells.size() == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, unindicted_spells.size() - 1);

    int random = dist(gen);
    spells.push_back(unindicted_spells[random]);
    unindicted_spells.erase(unindicted_spells.begin() + random);
} 



InterfaceSpell* SpellHand::getSpellByIndex(int index, InterfaceGameLogger& logger) {
    if (index == 0 || index > int(spells.size())) {
        logger.log("unexplored spells\n");
        return nullptr;
    } 
    return spells[index - 1];
}

void SpellHand::printSpells(InterfaceGameLogger& logger) {
    logger.log("Player spells:\n");
    for (size_t i = 0; i < spells.size(); ++i) { 
        logger.log(" [" + std::to_string(i+1) + "] " + spells[i]->getName() + "\n");
    }
}


void SpellHand::draw(sf::RenderWindow& window) {
    if (spells.empty()) return;

    const float CARD_WIDTH = 280.f / 1.5f;
    const float CARD_HEIGHT = 370.f / 1.5f;
    const float CARD_SPACING = 10.f;

    sf::Vector2u winSize = window.getSize();
    float panelX = winSize.x - CARD_WIDTH - 20.f;
    float panelY = 20.f;

    sf::Vector2f pos(panelX, panelY);

    for (size_t i = 0; i < spells.size(); ++i) {
        auto& spell = spells[i];

        sf::Sprite sprite = spell->get_sprite();
        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        
        const float MAX_SPRITE_SIZE = CARD_HEIGHT - 40.f;
        float scale = MAX_SPRITE_SIZE / std::max(textureSize.x, textureSize.y);
        
        float spriteWidth = textureSize.x * scale;
        float spriteX = pos.x + (CARD_WIDTH - spriteWidth) / 2;
        float spriteY = pos.y + 30.f;
        
        sprite.setPosition(spriteX, spriteY);
        sprite.setScale(scale, scale);

        window.draw(sprite);

        sf::Font font;
        static sf::Font defaultFont;
        defaultFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
        
        std::string spellText = std::to_string(i + 1) + ". " + spell->getName();
        sf::Text text(spellText, defaultFont, 16);
        text.setPosition(pos.x + 5, pos.y + 5);
        text.setFillColor(sf::Color::White);
        window.draw(text);
    

        pos.y += CARD_HEIGHT + CARD_SPACING;
    }
}

InterfaceSpell*  SpellHand::createSpellByName(const std::string& name) {
    if (name == "FireballSpell") return new FireballSpell();
    else if (name == "AbsorptionSpell") return new AbsorptionSpell();
    else if (name == "TrapSpell") return new TrapSpell();
    else if (name == "Arrise") return new Arrise();
    else if (name == "EnhancementSpell") return new EnhancementSpell();
    else {
        std::cout << "Unknown spell name: " << name << std::endl;
        return nullptr;
    }
}

void SpellHand::setSpellsByName(const std::vector<std::string>& newSpells) {
    for (auto& spell : spells) {
        delete spell;
    }
    for (auto& name_spell : newSpells) {
        InterfaceSpell* spell = createSpellByName(name_spell);
        if (spell) {
            spells.push_back(spell);
        }
    }
}

void SpellHand::setUnindictedSpellsByName(const std::vector<std::string>& newUnindictedSpells) {
    for (auto& spell : unindicted_spells) {
        delete spell;
    }
    for (auto& name_spell : newUnindictedSpells) {
        InterfaceSpell* spell = createSpellByName(name_spell);
        if (spell) {
            unindicted_spells.push_back(spell);
        }
    }
}

void SpellHand::clearAllSpells() {
    for (auto& spell : spells) {
        delete spell;
    }
    for (auto& spell : unindicted_spells) {
        delete spell;
    }
    spells.clear();
    unindicted_spells.clear();
}



void SpellHand::reset() {
    clearAllSpells();
    unindicted_spells.push_back(new FireballSpell());
    unindicted_spells.push_back(new AbsorptionSpell());
    unindicted_spells.push_back(new TrapSpell());
    unindicted_spells.push_back(new Arrise());
    unindicted_spells.push_back(new EnhancementSpell());
    addRandomSpell();
}


void SpellHand::moveFirstHalfToUnindicted() {
    if (spells.size() <= 1) return;
    
    int halfCount = spells.size() / 2;
    
    for (int i = 0; i < halfCount; i++) {
        unindicted_spells.push_back(spells[i]);
    }
    
    std::vector<InterfaceSpell*> remainingSpells;
    for (size_t i = halfCount; i < spells.size(); i++) {
        remainingSpells.push_back(spells[i]);
    }
    
    spells.clear();
    spells = remainingSpells;
}

void SpellHand::moveSpellToUnindicted(int index) {
    if (index < 0 || index >= (int)spells.size()) return;
    
    InterfaceSpell* spell = spells[index];
    unindicted_spells.push_back(spell);
    spells.erase(spells.begin() + index);
}

void SpellHand::moveSpellToLearned(int index) {
    if (index < 0 || index >= (int)unindicted_spells.size()) return;
    
    InterfaceSpell* spell = unindicted_spells[index];
    spells.push_back(spell);
    unindicted_spells.erase(unindicted_spells.begin() + index);
}


void SpellHand::setSpells(const std::vector<InterfaceSpell*>& newSpells) {
    for (auto& spell : spells) {
        bool foundInNew = false;
        for (auto newSpell : newSpells) {
            if (spell == newSpell) {
                foundInNew = true;
                break;
            }
        }
        if (!foundInNew) {
            delete spell;
        }
    }
    spells = newSpells;
}

void SpellHand::UpgradeEnhancementSpell(int new_level_spells) {
    for(auto i : spells) {
        if(i->getName() == "EnhancementSpell") {
            EnhancementSpell* ench = dynamic_cast<EnhancementSpell*>(i);
            ench->upgrade(new_level_spells);
            return;
        }
    }
    for(auto i : unindicted_spells) {
        if(i->getName() == "EnhancementSpell") {
            EnhancementSpell* ench = dynamic_cast<EnhancementSpell*>(i);
            ench->upgrade(new_level_spells);
            return;
        }
    }
}

int SpellHand::getLevelEnhancementSpell() {
    for(auto i : spells) {
        if(i->getName() == "EnhancementSpell") {
            EnhancementSpell* ench = dynamic_cast<EnhancementSpell*>(i);
            return ench->getbonusLevel();
        }
    }
    for(auto i : unindicted_spells) {
        if(i->getName() == "EnhancementSpell") {
            EnhancementSpell* ench = dynamic_cast<EnhancementSpell*>(i);
            return ench->getbonusLevel();
        }
    }
    return 0;
}
