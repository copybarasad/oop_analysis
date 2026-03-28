#include "EnemyController.hpp"
#include "Field.hpp"
#include "Player.hpp"

#include <iostream>

extern bool g_verbose;

#include <queue>
#include <limits>
#include <vector>
#include <algorithm>
#include <random>

std::vector<std::vector<int>>
EnemyController::buildDistanceMapToPlayer(const Field& field) {
    const int w = field.width();
    const int h = field.height();
    const int INF = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> dist(h, std::vector<int>(w, INF));
    std::queue<Position> q;
    bool hasSource = false;
        if (field.playerPos_.has_value()) {
            const Position playerPos = *field.playerPos_;
            dist[playerPos.y][playerPos.x] = 0;
            q.push(playerPos);
            hasSource = true;
        }
        for (const auto& a : field.allyOverlay_) {
            if (!field.isInside(a)) continue;
            if (dist[a.y][a.x] == 0) continue;
            dist[a.y][a.x] = 0;
            q.push(a);
            hasSource = true;
        }
        if (!hasSource) return dist;

    auto tryPush = [&](const Position& np, int nd) {
        if (!field.isInside(np)) return;
        if (!field.cellIsPassable(np)) return;
        if (field.findNestAt(np) != -1) return;
        if (field.hasAllyAt(np)) return;
        if (dist[np.y][np.x] <= nd) return;
        dist[np.y][np.x] = nd;
        q.push(np);
    };

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();
        int d = dist[cur.y][cur.x];

        tryPush(Position{cur.x,     cur.y - 1}, d + 1);
        tryPush(Position{cur.x,     cur.y + 1}, d + 1);
        tryPush(Position{cur.x - 1, cur.y    }, d + 1);
        tryPush(Position{cur.x + 1, cur.y    }, d + 1);
    }

    return dist;
}

void EnemyController::advanceEnemies(Field& field) {
    if (!field.playerPos_.has_value() || !field.player_.isAlive()) {
        return;
    }

    const int w = field.width();
    const int h = field.height();
    std::vector<std::vector<int>> dist = buildDistanceMapToPlayer(field);
    std::vector<std::vector<bool>> occupied(
        h, std::vector<bool>(w, false)
    );
    for (const auto& e : field.enemies_) {
        if (field.isInside(e.pos)) {
            occupied[e.pos.y][e.pos.x] = true;
        }
    }
    occupied[field.playerPos_->y][field.playerPos_->x] = true;
    for (const auto& a : field.allyOverlay_) {
        if (field.isInside(a)) occupied[a.y][a.x] = true;
    }
    field.lastPlayerDamage_ = 0;
    std::vector<int> order(field.enemies_.size());
    for (int i = 0; i < static_cast<int>(order.size()); ++i) {
        order[i] = i;
    }
    std::shuffle(order.begin(), order.end(), field.rng_);

    struct MoveRequest {
        int enemyIndex;
        Position from;
        Position primaryTo;
        bool hasAlternative;
        Position alternativeTo;
        bool resolved;
    };

    std::vector<MoveRequest> requests;
    requests.reserve(order.size());

    const Position deltas[4] = {
        { 0, -1 },
        { 0,  1 },
        {-1,  0 },
        { 1,  0 }
    };
    for (int ordIdx : order) {
        auto& entry = field.enemies_[ordIdx];
        if (!entry.enemy.isAlive()) {
            continue;
        }
            bool adjacentToTarget = false;
            for (const auto& d : deltas) {
                Position nearPos{ entry.pos.x + d.x, entry.pos.y + d.y };
                if (!field.isInside(nearPos)) continue;
                if (field.playerPos_.has_value() && nearPos == *field.playerPos_) {
                    adjacentToTarget = true; break;
                }
                if (field.hasAllyAt(nearPos)) { adjacentToTarget = true; break; }
            }

            if (adjacentToTarget) {
                if (entry.skipMoveNextTurn) {
                    entry.skipMoveNextTurn = false;
                }
                int dmg = entry.enemy.damage();
                for (const auto& d : deltas) {
                    Position nearPos{ entry.pos.x + d.x, entry.pos.y + d.y };
                    if (!field.isInside(nearPos)) continue;
                    if (field.playerPos_.has_value() && nearPos == *field.playerPos_) {
                        field.applyDamageToPlayer(dmg);
                        if (g_verbose) std::cout << "Enemy at (" << entry.pos.x << "," << entry.pos.y
                                  << ") hits PLAYER for " << dmg << " dmg\n";
                        break;
                    }
                    if (field.hasAllyAt(nearPos)) {
                        field.recordAllyDamageRequest(nearPos, dmg);
                        if (g_verbose) std::cout << "Enemy at (" << entry.pos.x << "," << entry.pos.y
                                  << ") hits ALLY at (" << nearPos.x << "," << nearPos.y
                                  << ") for " << dmg << " dmg\n";
                        break;
                    }
                }
                continue;
            }
        if (entry.skipMoveNextTurn) {
            entry.skipMoveNextTurn = false;
            continue;
        }

        const int curDist = dist[entry.pos.y][entry.pos.x];
        const bool unreachable = (curDist == std::numeric_limits<int>::max());

        struct Candidate {
            Position cell;
            int distVal;
        };
        std::vector<Candidate> better;
        std::vector<Candidate> equal;

        for (const auto& d : deltas) {
            Position np{ entry.pos.x + d.x, entry.pos.y + d.y };

            if (!field.isInside(np)) continue;
            if (!field.cellIsPassable(np)) continue;
            if (field.findNestAt(np) != -1) continue;
            if (field.hasAllyAt(np)) continue;
            if (np == *field.playerPos_) continue;

            int nd = dist[np.y][np.x];
            if (unreachable) {
                nd = curDist;
            }
            if (nd < curDist) {
                better.push_back({np, nd});
            } else if (nd == curDist) {
                equal.push_back({np, nd});
            }
        }

        if (better.empty() && equal.empty()) {
            continue;
        }
        auto cmpByDist = [](const Candidate& a, const Candidate& b){
            return a.distVal < b.distVal;
        };
        std::sort(better.begin(), better.end(), cmpByDist);
        std::sort(equal.begin(),  equal.end(),  cmpByDist);

        std::shuffle(better.begin(), better.end(), field.rng_);
        std::shuffle(equal.begin(),  equal.end(),  field.rng_);

        MoveRequest req;
        req.enemyIndex     = ordIdx;
        req.from           = entry.pos;
        req.primaryTo      = entry.pos;
        req.hasAlternative = false;
        req.alternativeTo  = entry.pos;
        req.resolved       = false;

        if (!better.empty()) {
            req.primaryTo = better[0].cell;
            if (better.size() > 1) {
                req.hasAlternative = true;
                req.alternativeTo  = better[1].cell;
            } else if (!equal.empty()) {
                req.hasAlternative = true;
                req.alternativeTo  = equal[0].cell;
            }
        } else {
            req.primaryTo = equal[0].cell;
            if (equal.size() > 1) {
                req.hasAlternative = true;
                req.alternativeTo  = equal[1].cell;
            }
        }

        requests.push_back(req);
    }
    std::vector<std::vector<std::vector<int>>> contenders(
        h, std::vector<std::vector<int>>(w)
    );

    for (int i = 0; i < static_cast<int>(requests.size()); ++i) {
        const MoveRequest& r = requests[i];
        if (field.isInside(r.primaryTo)) {
            contenders[r.primaryTo.y][r.primaryTo.x].push_back(i);
        }
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {

            auto& claimers = contenders[y][x];
            if (claimers.empty()) {
                continue;
            }

            if (occupied[y][x]) {
                continue;
            }

            int chosenReqIdx;
            if (claimers.size() == 1) {
                chosenReqIdx = claimers[0];
            } else {
                std::uniform_int_distribution<int> pick(
                    0,
                    static_cast<int>(claimers.size()) - 1
                );
                chosenReqIdx = claimers[pick(field.rng_)];
            }

            MoveRequest& winner = requests[chosenReqIdx];
            if (winner.resolved) {
                continue;
            }

            if (field.isInside(winner.from)) {
                occupied[winner.from.y][winner.from.x] = false;
            }

            occupied[y][x] = true;
            field.enemies_[winner.enemyIndex].pos = Position{x, y};

            if (field.at(field.enemies_[winner.enemyIndex].pos).isSlow()) {
                field.enemies_[winner.enemyIndex].skipMoveNextTurn = true;
            }

            winner.resolved = true;
        }
    }
    for (auto& req : requests) {
        if (req.resolved) {
            continue;
        }
        if (!req.hasAlternative) {
            continue;
        }

        const Position& alt = req.alternativeTo;
        if (!field.isInside(alt)) {
            continue;
        }

        if (!occupied[alt.y][alt.x]) {
            if (field.isInside(req.from)) {
                occupied[req.from.y][req.from.x] = false;
            }

            occupied[alt.y][alt.x] = true;
            field.enemies_[req.enemyIndex].pos = alt;

            if (field.at(field.enemies_[req.enemyIndex].pos).isSlow()) {
                field.enemies_[req.enemyIndex].skipMoveNextTurn = true;
            }

            req.resolved = true;
        }
    }
}
