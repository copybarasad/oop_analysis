#include "world.hpp"

#include <algorithm>
#include <random>
#include <utility>

void remove_half_hand_random(Hand& hand) {
    std::vector<std::size_t> idx;
    for (std::size_t i=0;i<hand.capacity();++i) if (hand.has(i)) idx.push_back(i);
    if (idx.empty()) return;
    std::random_device rd; std::mt19937 g(rd());
    std::shuffle(idx.begin(), idx.end(), g);
    std::size_t to_remove = idx.size()/2;
    for (std::size_t k=0; k<to_remove; ++k) hand.remove(idx[k]);
}

void proceed_to_next_level(World& world) {
    world.level = std::max(1, world.level + 1);

    int newW = std::min(25, world.field.getWidth()  + 2);
    int newH = std::min(25, world.field.getHeight() + 2);
    Field newField(newW, newH);

    for (int x=0; x<newW; ++x) { newField.at(x,0).setType(CellType::Wall); newField.at(x,newH-1).setType(CellType::Wall); }
    for (int y=0; y<newH; ++y) { newField.at(0,y).setType(CellType::Wall); newField.at(newW-1,y).setType(CellType::Wall); }

    int healTo = (world.baseHeroHp > 0 ? world.baseHeroHp : world.hero.getHp());
    Hero newHero(healTo, world.hero.getDmg(), Position(1,1));

    world.enemies.clear();
    world.spawners.clear();
    world.traps.clear();
    world.towers.clear();
    world.allies.clear();

    int spCount = std::min(5, 2 + world.level/2);
    int period  = std::max(2, 5 - world.level/3);
    int spHp = 10 + 2 * world.level;
    
    std::vector<Position> spots = {{1,newH-2},{newW-2,1},{newW-2,newH-2},{newW/2,newH/2},{newW/2,1}};
    for (int i=0; i<spCount && i<(int)spots.size(); ++i) world.spawners.emplace_back(spots[i], period, spHp);

    int baseHp = 3 + world.level*2, baseDmg = 1 + world.level;
    std::vector<Position> enemyStarts = { {newW-2,newH-2}, {newW-3,newH-3}, {newW-2,1}, {1,newH-2} };
    for (int i=0; i<std::min(2+world.level, (int)enemyStarts.size()); ++i)
        world.enemies.emplace_back(baseHp, baseDmg, enemyStarts[i]);

    world.field = std::move(newField);
    world.hero  = std::move(newHero);
    
    for (auto& s : world.spawners) {
        if (s.hasTower()) continue;
        static const Position order[4]{{1,0},{0,1},{-1,0},{0,-1}};
        Position sPos = s.getPos();
        for (auto d : order) {
            Position cand{sPos.getX() + d.getX(), sPos.getY() + d.getY()};
            if (!world.field.inBounds(cand.getX(),cand.getY())) continue;
            if (!world.field.passable(cand.getX(),cand.getY())) continue;
            if (world.hero.getPosition() == cand) continue;
            bool blocked = false;
            for (auto const& e: world.enemies) if (!e.isDead() && e.getPosition()==cand) { blocked=true; break; }
            if (blocked) continue;
            for (auto const& a: world.allies) if (!a.isDead() && a.getPosition()==cand) { blocked=true; break; }
            if (blocked) continue;
            for (auto const& t: world.towers) if (t.getPos()==cand) { blocked=true; break; }
            if (blocked) continue;
            for (auto const& sp: world.spawners) if (sp.getPos()==cand) { blocked=true; break; }
            if (blocked) continue;
            if (trap_index_at(world, cand) >= 0) continue;
            world.towers.push_back(Tower(cand, std::max(1, world.level), 2));
            s.markTower();
            break;
        }
    }
    
    remove_half_hand_random(world.hand);
}

