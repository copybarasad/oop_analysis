#include "GameState.h"
#include "GameExceptions.h"
#include <sstream>

namespace {
SpellType inferSpellTypeFromName(const std::string& name) {
    if (name.find("Meteor") != std::string::npos) {
        return SpellType::AREA_DAMAGE;
    }
    if (name.find("Bear Trap") != std::string::npos || name.find("Trap") != std::string::npos) {
        return SpellType::TRAP;
    }
    if (name.find("Summon") != std::string::npos) {
        return SpellType::SUMMON;
    }
    if (name.find("Empower") != std::string::npos || name.find("Enhance") != std::string::npos) {
        return SpellType::ENHANCE;
    }
    return SpellType::DIRECT_DAMAGE;
}
}

void SpellState::serialize(std::ostream& out) const {
    out << static_cast<int>(type) << " " << name << "\n";
}

SpellState SpellState::deserialize(std::istream& in) {
    SpellState spell;
    int typeInt;
    if (!(in >> typeInt)) {
        throw CorruptedDataError("Failed to read spell type");
    }
    spell.type = static_cast<SpellType>(typeInt);
    in.get();
    if (!std::getline(in, spell.name)) {
        throw CorruptedDataError("Failed to read spell name");
    }
    return spell;
}

void PlayerState::serialize(std::ostream& out) const {
    out << x << " " << y << " " 
        << health << " " << maxHealth << " "
        << damage << " " << baseMovementSpeed << " "
        << canMove << " " << isSlowed << "\n";
}

PlayerState PlayerState::deserialize(std::istream& in) {
    PlayerState state;
    if (!(in >> state.x >> state.y >> state.health >> state.maxHealth 
          >> state.damage >> state.baseMovementSpeed >> state.canMove >> state.isSlowed)) {
        throw CorruptedDataError("Failed to read player state");
    }
    return state;
}

void EnemyState::serialize(std::ostream& out) const {
    out << x << " " << y << " " << health << " " << damage << "\n";
}

EnemyState EnemyState::deserialize(std::istream& in) {
    EnemyState state;
    if (!(in >> state.x >> state.y >> state.health >> state.damage)) {
        throw CorruptedDataError("Failed to read enemy state");
    }
    return state;
}

void BuildingState::serialize(std::ostream& out) const {
    out << x << " " << y << " " << health << " " << maxHealth << " "
        << turnCounter << " " << spawnCooldown << "\n";
}

BuildingState BuildingState::deserialize(std::istream& in) {
    BuildingState state;
    if (!(in >> state.x >> state.y >> state.health >> state.maxHealth 
          >> state.turnCounter >> state.spawnCooldown)) {
        throw CorruptedDataError("Failed to read building state");
    }
    return state;
}

void TowerState::serialize(std::ostream& out) const {
    out << x << " " << y << " " << health << " " << maxHealth << " "
        << attackRange << " " << attackDamage << " "
        << attackCooldown << " " << cooldownCounter << "\n";
}

TowerState TowerState::deserialize(std::istream& in) {
    TowerState state;
    if (!(in >> state.x >> state.y >> state.health >> state.maxHealth 
          >> state.attackRange >> state.attackDamage 
          >> state.attackCooldown >> state.cooldownCounter)) {
        throw CorruptedDataError("Failed to read tower state");
    }
    return state;
}

void AllyState::serialize(std::ostream& out) const {
    out << x << " " << y << " " << health << " " << damage << " " << shouldSkip << "\n";
}

AllyState AllyState::deserialize(std::istream& in) {
    AllyState state;
    if (!(in >> state.x >> state.y >> state.health >> state.damage >> state.shouldSkip)) {
        throw CorruptedDataError("Failed to read ally state");
    }
    return state;
}

void TrapState::serialize(std::ostream& out) const {
    out << x << " " << y << " " << damage << "\n";
}

TrapState TrapState::deserialize(std::istream& in) {
    TrapState state;
    if (!(in >> state.x >> state.y >> state.damage)) {
        throw CorruptedDataError("Failed to read trap state");
    }
    return state;
}

void FieldState::serialize(std::ostream& out) const {
    out << width << " " << height << "\n";
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            out << cellTypes[y][x];
            if (x < width - 1) out << " ";
        }
        out << "\n";
    }
}

FieldState FieldState::deserialize(std::istream& in) {
    FieldState state;
    if (!(in >> state.width >> state.height)) {
        throw CorruptedDataError("Failed to read field dimensions");
    }
    
    state.cellTypes.resize(state.height);
    for (int y = 0; y < state.height; ++y) {
        state.cellTypes[y].resize(state.width);
        for (int x = 0; x < state.width; ++x) {
            if (!(in >> state.cellTypes[y][x])) {
                throw CorruptedDataError("Failed to read cell type at position (" + 
                                       std::to_string(x) + "," + std::to_string(y) + ")");
            }
        }
    }
    
    return state;
}

void GameState::serialize(std::ostream& out) const {
    out << "VERSION " << version << "\n";
    out << "LEVEL " << currentLevel << "\n";
    out << "TURN " << currentTurn << "\n";
    out << "ENEMIES_KILLED " << enemiesKilled << "\n";
    out << "HAS_REVIVE " << hasUsedRevive << "\n";
    
    out << "FIELD\n";
    field.serialize(out);
    
    out << "PLAYER\n";
    player.serialize(out);
    
    out << "ENEMIES " << enemies.size() << "\n";
    for (const auto& enemy : enemies) {
        enemy.serialize(out);
    }
    
    out << "BUILDINGS " << buildings.size() << "\n";
    for (const auto& building : buildings) {
        building.serialize(out);
    }
    
    out << "TOWERS " << towers.size() << "\n";
    for (const auto& tower : towers) {
        tower.serialize(out);
    }
    
    out << "ALLIES " << allies.size() << "\n";
    for (const auto& ally : allies) {
        ally.serialize(out);
    }
    
    out << "TRAPS " << traps.size() << "\n";
    for (const auto& trap : traps) {
        trap.serialize(out);
    }
    
    out << "SPELLS " << handSpells.size() << "\n";
    for (const auto& spell : handSpells) {
        spell.serialize(out);
    }
    
    out << "UPGRADES " << upgrades.size() << "\n";
    for (const auto& upgrade : upgrades) {
        out << static_cast<int>(upgrade) << "\n";
    }
}

GameState GameState::deserialize(std::istream& in) {
    GameState state;
    std::string keyword;
    
    in >> keyword;
    if (keyword != "VERSION") {
        throw CorruptedDataError("Expected VERSION keyword");
    }
    in >> state.version;
    
    in >> keyword >> state.currentLevel;
    if (keyword != "LEVEL") {
        throw CorruptedDataError("Expected LEVEL keyword");
    }
    
    in >> keyword >> state.currentTurn;
    if (keyword != "TURN") {
        throw CorruptedDataError("Expected TURN keyword");
    }
    
    in >> keyword >> state.enemiesKilled;
    if (keyword != "ENEMIES_KILLED") {
        throw CorruptedDataError("Expected ENEMIES_KILLED keyword");
    }
    
    in >> keyword >> state.hasUsedRevive;
    if (keyword != "HAS_REVIVE") {
        throw CorruptedDataError("Expected HAS_REVIVE keyword");
    }
    
    in >> keyword;
    if (keyword != "FIELD") {
        throw CorruptedDataError("Expected FIELD keyword");
    }
    state.field = FieldState::deserialize(in);
    
    in >> keyword;
    if (keyword != "PLAYER") {
        throw CorruptedDataError("Expected PLAYER keyword");
    }
    state.player = PlayerState::deserialize(in);
    
    size_t enemyCount;
    in >> keyword >> enemyCount;
    if (keyword != "ENEMIES") {
        throw CorruptedDataError("Expected ENEMIES keyword");
    }
    for (size_t i = 0; i < enemyCount; ++i) {
        state.enemies.push_back(EnemyState::deserialize(in));
    }
    
    size_t buildingCount;
    in >> keyword >> buildingCount;
    if (keyword != "BUILDINGS") {
        throw CorruptedDataError("Expected BUILDINGS keyword");
    }
    for (size_t i = 0; i < buildingCount; ++i) {
        state.buildings.push_back(BuildingState::deserialize(in));
    }
    
    size_t towerCount;
    in >> keyword >> towerCount;
    if (keyword != "TOWERS") {
        throw CorruptedDataError("Expected TOWERS keyword");
    }
    for (size_t i = 0; i < towerCount; ++i) {
        state.towers.push_back(TowerState::deserialize(in));
    }
    
    size_t allyCount;
    in >> keyword >> allyCount;
    if (keyword != "ALLIES") {
        throw CorruptedDataError("Expected ALLIES keyword");
    }
    for (size_t i = 0; i < allyCount; ++i) {
        state.allies.push_back(AllyState::deserialize(in));
    }
    
    size_t trapCount;
    in >> keyword >> trapCount;
    if (keyword != "TRAPS") {
        throw CorruptedDataError("Expected TRAPS keyword");
    }
    for (size_t i = 0; i < trapCount; ++i) {
        state.traps.push_back(TrapState::deserialize(in));
    }
    
    size_t spellCount;
    in >> keyword >> spellCount;
    if (keyword != "SPELLS") {
        throw CorruptedDataError("Expected SPELLS keyword");
    }
    in.ignore();
    for (size_t i = 0; i < spellCount; ++i) {
        if (state.version == "1.0") {
            std::string spellName;
            std::getline(in, spellName);
            if (spellName.empty()) {
                throw CorruptedDataError("Failed to read legacy spell name");
            }
            SpellState legacySpell;
            legacySpell.name = spellName;
            legacySpell.type = inferSpellTypeFromName(spellName);
            state.handSpells.push_back(legacySpell);
        } else {
            state.handSpells.push_back(SpellState::deserialize(in));
        }
    }
    
    size_t upgradeCount;
    in >> keyword >> upgradeCount;
    if (keyword != "UPGRADES") {
        throw CorruptedDataError("Expected UPGRADES keyword");
    }
    for (size_t i = 0; i < upgradeCount; ++i) {
        int upgradeInt;
        in >> upgradeInt;
        state.upgrades.push_back(static_cast<UpgradeType>(upgradeInt));
    }
    
    return state;
}
