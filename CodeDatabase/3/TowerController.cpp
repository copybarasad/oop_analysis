#include "GameClass/TowerController.hpp"
#include "GameObjects/Spells/Target.hpp"

TowerController::TowerController(int level, MapController& map) {
    loadLevel(level, map);
    targetSpell = std::make_unique<Target>(0);
}


std::vector<TowerDTO> TowerController::toDTO() {
    std::vector<TowerDTO> res;

    for (auto& at : attackTowers) {
        TowerDTO dto;
        dto.coolDown = at.getCoolDown();
        dto.coord = at.getPosition();
        dto.hp = at.getHealthPoints();
        dto.isAttack = true;
        dto.maxHp = at.getMaxHP();
        dto.steps = at.getSteps();

        res.push_back(dto);
    }

    for (auto& sp : spawnTowers) {
        TowerDTO dto;
        dto.coolDown = sp.getCoolDown();
        dto.coord = sp.getPosition();
        dto.isAttack = false;
        dto.steps = sp.getSteps();
        dto.enemyStats = sp.getEnemyStats();
        res.push_back(dto);
    }
    
    return res;
}



void TowerController::loadLevel(int lvl, MapController& map) {
    spawnTowers.clear();
    attackTowers.clear();

    std::vector<TowerDTO> ts = LM.loadLevelTowers(lvl);
    for (auto& t : ts) {
        if (t.isAttack) {
            AttackTower at = AttackTower();
            at.setMaxHP(t.maxHp);
            at.setHealthPoints(t.hp);
            at.setCoolDown(t.coolDown);
            at.setSteps(t.steps);
            at.setPosition(t.coord);

            attackTowers.push_back(at);
            map.getField(at.getPosition())->setStatus(FieldType::ATTACK_TOWER);
        } else {
            SpawnTower sp = SpawnTower();
            sp.setSteps(t.steps);
            sp.setEnemyStats(t.enemyStats);
            sp.setCoolDown(t.coolDown);
            sp.setPositoin(t.coord);

            spawnTowers.push_back(sp);
            map.getField(sp.getPosition())->setStatus(FieldType::SPAWN_TOWER);
        }
    }
}


void TowerController::makeAllStep(MapController& map, EnemyController& ec, Player& player, StatisticDTO& stats) {
    removeDeadTowers(map, stats);

    for (auto it = spawnTowers.begin(); it != spawnTowers.end(); ) {
        if (!it->onCoolDown()) {
            Coordinates pos = it->getPosition();
            std::vector<Coordinates> ways = {
                {pos.x - 1, pos.y},
                {pos.x + 1, pos.y},
                {pos.x, pos.y - 1},
                {pos.x, pos.y + 1}
            };
            for (const auto& way : ways) {
                if (map.getField(way.x, way.y)) {
                    if (map.getField(way.x, way.y)->getStatus() == FieldType::FIELD_DEFAULT) {
                        Enemy e = it->spawnEnemy(way);
                        ec.addEnemy(e, map);
                        
                        break;
                    }
                }
            }

        } else {
            it->addStep();
        }
        it++;
    }

    for (auto it = attackTowers.begin(); it != attackTowers.end(); ) {
        Coordinates pos = it->getPosition();
        Coordinates playerPos = player.getPosition();
        bool playerIsVictim = (std::abs(pos.x - playerPos.x) + std::abs(pos.y - playerPos.y)) <= 2;
        it->addStep();
        if (playerIsVictim) {
            if (!it->onCoolDown()) {
                auto c = targetSpell->use(playerPos);
                player.minusHealthPoints(c[0].damage);
            }
        }

        it++;
    }

    needMove = false;
}


void TowerController::removeDeadTowers(MapController& map, StatisticDTO& stats) {
    for (auto it = attackTowers.begin(); it != attackTowers.end(); ) {
        if (it->getHealthPoints() <= 0) {
            map.getField(it->getPosition())->toInitStatus();
            it = attackTowers.erase(it);
            stats.points += 100;
            stats.bonus = true;
        } else {
            ++it;
        }
    }
}


AttackTower* TowerController::getAttackTower(Coordinates position) {
    for (AttackTower& t : attackTowers) {
        if (t.getPosition() == position) {
            return &t;
        }
    }
    return nullptr;
}


SpawnTower* TowerController::getSpawnTower(Coordinates position) {
    for (SpawnTower& t : spawnTowers) {
        if (t.getPosition() == position) {
            return &t;
        }
    }
    return nullptr;
}


void TowerController::makeActive() {
    needMove = true;
}

bool TowerController::isActive() {
    return needMove;
}