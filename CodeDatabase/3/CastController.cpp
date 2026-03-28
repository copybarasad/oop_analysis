#include "GameClass/CastController.hpp"
#include "GameClass/GameClass.hpp"


CastController::CastController(int level) : hand(LM.loadHand(level)) {
    if (level == 0) {
        contracts = LM.loadTraps();
    }
}

void CastController::goToNextLevel() {
    contracts.clear();
    hand.shakeHand();
}

void CastController::addSpell() {
    hand.addRandomSpell();
}

void CastController::upgradeSpell(SpellType spell) {
    switch (spell)
    {
    case SpellType::AoESpell :
        hand.upAreaLvl();
        break;
    case SpellType::TargetSpell :
        hand.upTargetLvl();
        break;
        case SpellType::TrapSpell :
        hand.upTrapLvl();
        break;
    }
}

bool CastController::canUseHere(FieldType ft) {
    return hand.getSpellByIndex(spellIndex)->canUseHere(ft);
}

void CastController::executeContracts(MapController& map, EnemyController& enemies, TowerController& towers, Player& player) {
    for (size_t i = 0; i < contracts.size();) {
        
        SpellContract& c = contracts[i];
        if (c.coord.x < 0 || c.coord.y < 0 || c.coord.x >= map.getWidth() || c.coord.y >= map.getHeight()) {
            contracts.erase(contracts.begin() + i);
            continue;
        }
        FieldType ft = map.getField(c.coord)->getStatus();
        bool remove = true;

        if (c.longLive) {
            if (ft == FieldType::FIELD_DEFAULT) {
                map.getField(c.coord)->setStatus(FieldType::FIELD_TRAP);
                remove = false;
            } else if (ft == FieldType::ENEMY) {
                enemies.getEnemy(c.coord)->minusHealthPoints(c.damage);
            } else if (ft == FieldType::PLAYER) {
                player.minusHealthPoints(c.damage);
            } else remove = false;
        } else {
            switch (ft) {
                case FieldType::ENEMY:
                    enemies.getEnemy(c.coord)->minusHealthPoints(c.damage);
                    break;
                case FieldType::ATTACK_TOWER:
                    towers.getAttackTower(c.coord)->minusHealthPoints(c.damage);
                    break;
                case FieldType::PLAYER:
                    player.minusHealthPoints(c.damage);
                    break;
                default:
                    break;
            }
        }

        if (remove) {
            contracts.erase(contracts.begin() + i);
        } else {
            i++;
        }
    }
}


void CastController::startCast(Motion cmd, Coordinates playerPos, int mapH, int mapW) {
    switch (cmd) {
        case Motion::StartCast1:
            spellIndex = 0;
            break;
        case Motion::StartCast2:
            spellIndex = 1;
            break;
        case Motion::StartCast3:
            spellIndex = 2;
            break;
        case Motion::StartCast4:
            spellIndex = 3;
            break;
        case Motion::StartCast5:
            spellIndex = 4;
            break;
        default:
            break;
    }
    startSpell(playerPos, mapH, mapW);
}

void CastController::startSpell(Coordinates playerPos, int mapH, int mapW) {
    dto.castRange = hand.getSpellByIndex(spellIndex)->getCastRangeCoordinates(playerPos, mapW, mapH);
    dto.target = playerPos;
}

void CastController::moveTarget(Motion cmd) {
    Coordinates delta;

    switch (cmd)
    {
    case Motion::MoveUpCast:
        delta = {0, -1};
        break;
    case Motion::MoveDownCast:
        delta = {0, 1};
        break;
    case Motion::MoveLeftCast:
        delta = {-1, 0};
        break;
    case Motion::MoveRightCast:
        delta = {1, 0};
        break;
    default:
        break;
    }

    dto.target.x += delta.x;
    dto.target.y += delta.y;
}

Coordinates CastController::getTarget() {
    return dto.target;
}

void CastController::confirm() {
    std::vector<SpellContract> newContracts = hand.getSpellByIndex(spellIndex)->use(dto.target);
    contracts.insert(contracts.end(), newContracts.begin(), newContracts.end());
    hand.deleteSpellByIndex(spellIndex);
}


CastDTO CastController::toDTO() {
    dto.hand = hand.toDTO();
    dto.traps = contracts;
    return dto;
}

std::vector<Coordinates> CastController::getAllowedSteps() {
    std::array<Coordinates, 4> deltas = {
        Coordinates{0, -1},
        Coordinates{0,  1},
        Coordinates{-1, 0},
        Coordinates{1,  0}
    };
    std::vector<Coordinates> result;

    for (auto& d : deltas) {
        if (std::find(dto.castRange.begin(), dto.castRange.end(), Coordinates{dto.target.x + d.x, dto.target.y + d.y}) != dto.castRange.end()) {
            result.push_back(d);
        }
    }

    return result;
}

int CastController::handSize() {
    return hand.size();
}