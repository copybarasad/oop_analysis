#include "turns.hpp"

#include <algorithm>

#include "ally.hpp"
#include "world.hpp"

bool melee_attack(World& w, std::vector<Position>& to_redraw) {
    static const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    for (const auto& d : dirs) {
        Position p = w.hero.getPosition() + d;

        int ei = -1;
        if (is_enemy_at(w, p, &ei)) {
            w.enemies[ei].takeDamage(w.hero.getDmg());
            if (w.enemies[ei].isDead()) {
                w.kills += 1;
                to_redraw.push_back(p);
            }
            return true;
        }

        int si = spawner_index_at(w, p);
        if (si >= 0) {
            w.spawners[si].takeDamage(w.hero.getDmg());
            if (w.spawners[si].isDead()) {
                to_redraw.push_back(p);
            }
            return true;
        }
    }
    return false;
}

bool ranged_attack(World& w, std::vector<Position>& to_redraw) {
    static const Position dirs[4] {{1,0},{-1,0},{0,1},{0,-1}};
    int rdmg = std::max(1, w.hero.getDmg() - w.hero.getDmg()/2); 

    for (const auto& d : dirs) {
        Position p = w.hero.getPosition();
        for (int step = 0; step < 3; ++step) {
            p = p + d;
            if (!w.field.inBounds(p.getX(), p.getY()) || !w.field.passable(p.getX(), p.getY())) break;

            int ei = -1;
            if (is_enemy_at(w, p, &ei)) {
                w.enemies[ei].takeDamage(rdmg);
                if (w.enemies[ei].isDead()) {
                    w.kills += 1;
                    to_redraw.push_back(p);
                }
                return true;
            }

            int si = spawner_index_at(w, p);
            if (si >= 0) {
                w.spawners[si].takeDamage(rdmg);
                if (w.spawners[si].isDead()) {
                    to_redraw.push_back(p);
                }
                return true;
            }
        }
    }
    return false;
}

void enemies_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& e : w.enemies) {
        if (e.isDead()) continue;
        Position from = e.getPosition();

        Position step = Enemy::chooseStep(e.getPosition(), w.hero.getPosition());
        Position dst{from.getX() + step.getX(), from.getY() + step.getY()};
        if (!w.field.inBounds(dst.getX(),dst.getY())) continue;

        if (dst == w.hero.getPosition()) {
            w.hero.takeDamage(e.getDmg());
            continue;
        }

        int allyIdx = -1;
        for (int i=0; i<(int)w.allies.size(); ++i) {
            if (!w.allies[i].isDead() && w.allies[i].getPos() == dst) { allyIdx = i; break; }
        }
        if (allyIdx >= 0) {
            w.allies[allyIdx].takeDamage(e.getDmg());
            to_redraw.push_back(dst);
            if (w.allies[allyIdx].isDead()) {
                e.setPos(dst);
                to_redraw.push_back(from);
            }
            continue;
        }

        int trapIdx = trap_index_at(w, dst);
        if (trapIdx >= 0) {
            int alive_before = alive_enemies(w);
            e.takeDamage(w.traps[trapIdx].getDamage());
            w.traps.erase(w.traps.begin() + trapIdx);
            to_redraw.push_back(dst);

            if (!e.isDead()) {
                if (!cell_free_for_enemy(w, dst)) continue;
                e.setPos(dst);
                to_redraw.push_back(from);
            } else {
                int alive_after = alive_enemies(w);
                if (alive_after < alive_before) w.kills += (alive_before - alive_after);
                to_redraw.push_back(from);
            }
            continue;
        }

        if (!cell_free_for_enemy(w, dst)) continue;
        e.setPos(dst);
        to_redraw.push_back(from);
        to_redraw.push_back(dst);
    }
}

void spawners_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& s : w.spawners) {
        auto spawn_pos = s.onTurn([&](const Position& p){ return cell_free_for_enemy(w,p); });
        if (spawn_pos) {
            w.enemies.emplace_back(Enemy{6,1 + w.level,*spawn_pos});
            to_redraw.push_back(*spawn_pos);
        }
    }
}

void towers_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& t : w.towers) {
        t.onTurn();
        (void)t.tryAttack(w.hero, w.field, w.enemies, w.spawners, w.allies, &to_redraw);
    }
}

void allies_turn(World& w, std::vector<Position>& to_redraw) {
    for (auto& a : w.allies) {
        if (a.isDead()) continue;

        int ei = a.closestEnemyIndex(w.enemies);
        if (ei < 0) continue;

        Position epos = w.enemies[ei].getPosition();
        int dist = Ally::manh(a.getPos(), epos);

        if (dist == 1) {
            w.enemies[ei].takeDamage(a.getDmg());
            if (w.enemies[ei].isDead()) { ++w.kills; to_redraw.push_back(epos); }
            continue;
        }

        Position step = Ally::stepToward(a.getPos(), epos);
        Position dst(a.getPos().getX() + step.getX(), a.getPos().getY() + step.getY());
        if (cell_free_for_ally(w, dst)) {
            Position from = a.getPos();
            a.setPos(dst);
            to_redraw.push_back(from);
            to_redraw.push_back(dst);
        }
    }
}

