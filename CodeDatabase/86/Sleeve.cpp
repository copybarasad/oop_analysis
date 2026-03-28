#include "Sleeve.hpp"
#include "GameWorld.hpp"
#include "GameContext.hpp"
#include "Character.hpp" 
#include "Player.hpp"
#include "Logger.hpp"
#include "Position.hpp"
#include <iostream>
#include <random>

Sleeve::Sleeve(int n) : maxSize(n) {
    addRandomSpell();
}

void Sleeve::addRandomSpell() {
    if (cards.size() >= maxSize) {
        return; 
    }
    
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(0, 1);
    
    if (dist(gen) == 0) {
        cards.push_back(std::make_unique<IdirectDamage>(15, 6));
    } else {
        cards.push_back(std::make_unique<IsquareDamage>(10, 6));
    }
}

void Sleeve::buyNewSpell(int stars, Player& player) {
    if (stars >= 5 && cards.size() < maxSize) {
        addRandomSpell();
    }
}

void Sleeve::showCards() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i << ") " << cards[i]->name() << " | ";
    }
    std::cout << std::endl;
}

int Sleeve::getSize() const {
    return static_cast<int>(cards.size());
}

bool Sleeve::useSpell(int index, GameWorld& world, GameContext& ctx, const Position& targetPos) {
    if (index < 0 || index >= static_cast<int>(cards.size())) {
        ctx.logger_.warning("Invalid spell index!");
        return false;
    }
    
    bool success = cards[index]->cast(world, ctx, targetPos);

    if(success){
        cards.erase(cards.begin() + index);
    }

    return success;
}


bool IdirectDamage::cast(GameWorld& world, GameContext& ctx, const Position& targetPos) {
    Player& player = world.getPlayer();
    MainEnemy& mainEnemy = world.getMainEnemy();

    int dist = CalcDist(player.getPos(), targetPos);
    if (dist > range_) {
        ctx.logger_.warning("Target out of range!");
        return false;
    }

    bool hit = false;

    if (mainEnemy.getPos().x == targetPos.x && mainEnemy.getPos().y == targetPos.y) {
        mainEnemy.takeDamage(damage_);
        ctx.logger_.action("Hit main enemy for " + std::to_string(damage_) + " damage!");
        hit = true;
    }

    if (auto* tower = world.getConstruction()) {
        if (tower->getPos().x == targetPos.x && tower->getPos().y == targetPos.y) {
            tower->takeDamage(damage_);
            ctx.logger_.action("Hit tower for " + std::to_string(damage_) + " damage!");
            hit = true;
        }
    }

    for (auto& enemy : world.getEnemies()) {
        if (enemy->getPos().x == targetPos.x && enemy->getPos().y == targetPos.y) {
            enemy->takeDamage(damage_);
            ctx.logger_.action("Hit enemy for " + std::to_string(damage_) + " damage!");
            hit = true;
            break;
        }
    }

    if (!hit) {
        ctx.logger_.warning("No valid target at position!");
        return false;
    }

    return true;
}

bool IsquareDamage::cast(GameWorld& world, GameContext& ctx, const Position& targetPos) {
    Player& player = world.getPlayer();
    MainEnemy& mainEnemy = world.getMainEnemy();

    int dist = CalcDist(player.getPos(), targetPos);
    if (dist > range_) {
        ctx.logger_.warning("Target out of range!");
        return false;
    }

    bool hit = false;

    for (int dy = 0; dy < 2; ++dy) {
        for (int dx = 0; dx < 2; ++dx) {
            int x = targetPos.x + dx;
            int y = targetPos.y + dy;

            if (x < 0 || x >= world.getGrid().getWidth() || y < 0 || y >= world.getGrid().getHeight()) {
                continue;
            }

            if (mainEnemy.getPos().x == x && mainEnemy.getPos().y == y) {
                mainEnemy.takeDamage(damage_);
                hit = true;
            }

            if (auto* tower = world.getConstruction()) {
                if (tower->getPos().x == x && tower->getPos().y == y) {
                    tower->takeDamage(damage_);
                    hit = true;
                }
            }

            for (auto& enemy : world.getEnemies()) {
                if (enemy->getPos().x == x && enemy->getPos().y == y) {
                    enemy->takeDamage(damage_);
                    hit = true;
                }
            }
        }
    }

    if (hit) {
        ctx.logger_.action("AoE spell hit targets for " + std::to_string(damage_) + " damage!");
    } else {
        ctx.logger_.info("AoE spell hit empty area");
    }

    return true;
}

void IdirectDamage::save(std::ostream& os) const {
    os << damage_ << " " << range_ << " ";
}
void IdirectDamage::load(std::istream& is) {
    is >> damage_ >> range_;
}

void IsquareDamage::save(std::ostream& os) const {
    os << damage_ << " " << range_ << " ";
}
void IsquareDamage::load(std::istream& is) {
    is >> damage_ >> range_;
}


void Sleeve::save(std::ostream& os) const {
    os << cards.size() << "\n";
    for (const auto& card : cards) {
        os << card->getTypeID() << " "; 
        card->save(os);                 
        os << "\n";
    }
}

void Sleeve::load(std::istream& is) {
    cards.clear(); 
    int size;
    is >> size;
    
    for (int i = 0; i < size; ++i) {
        int typeID;
        is >> typeID;
        
        std::unique_ptr<IspellCard> newCard;
        
        if (typeID == 1) {
            newCard = std::make_unique<IdirectDamage>(0, 0); 
        } else if (typeID == 2) {
            newCard = std::make_unique<IsquareDamage>(0, 0);
        }
        
        if (newCard) {
            newCard->load(is); 
            cards.push_back(std::move(newCard));
        }
    }
}