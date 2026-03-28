#include "spawner-building.hpp"
#include "../constants.hpp"
#include "../map/map.hpp"

SpawnerBuilding::SpawnerBuilding(int hp) : GenericBuilding(hp) {
    m_TickCount = 0;
    m_SpawnRate = 1;
}

SpawnerBuilding::SpawnerBuilding(const SpawnerBuilding &building)
    : GenericBuilding(building) {}

SpawnerBuilding::~SpawnerBuilding() {
    if (m_Enemy != nullptr) {
        delete m_Enemy;
    }
}

Building *SpawnerBuilding::clone() { return new SpawnerBuilding(*this); }

void SpawnerBuilding::update() {
    m_TickCount = (m_TickCount + 1) % m_SpawnRate;

    if (m_TickCount != 0) {
        return;
    }

    for (int i = 0; i < BUILDING_SPAWNER_MAX_ATTEMPTS; i++) {
        Field *field = m_Map->randomNearby(m_Pos);
        Position spawnPoint = m_Map->locateField(field);

        if (m_Map->canSpawnAt(spawnPoint)) {
            Enemy *enemy = new Enemy(*m_Enemy);
            m_Map->spawn(enemy, spawnPoint);
            break;
        }
    }
}

void SpawnerBuilding::setEnemy(const Enemy *enemy) {
    m_Enemy = new Enemy(*enemy);
}

void SpawnerBuilding::setSpawnRate(int spawnRate) {
    if (m_SpawnRate > 0) {
        m_SpawnRate = spawnRate;
    }
}

void SpawnerBuilding::setMap(Map *map) { m_Map = map; }

std::string SpawnerBuilding::chr() { return "#"; }
Color SpawnerBuilding::color() { return Color::red(); }

bytedump SpawnerBuilding::dump() {
    bytedump bytes;
    bytes.reserve(16);

    bytes.push_back(static_cast<std::byte>(SPAWNER_BUILDING_BYTE));
    bytes.push_back(static_cast<std::byte>(m_HP));
    bytes.push_back(static_cast<std::byte>(m_TickCount));
    bytes.push_back(static_cast<std::byte>(m_SpawnRate));

    // 7 bytes for enemy.
    bytedump enemyBytes = m_Enemy->dump();
    bytes.insert(bytes.end(), enemyBytes.begin(), enemyBytes.end());

    bytes.push_back(static_cast<std::byte>(0));
    bytes.push_back(static_cast<std::byte>(0));
    bytes.push_back(static_cast<std::byte>(0));
    bytes.push_back(static_cast<std::byte>(m_Pos.x));
    bytes.push_back(static_cast<std::byte>(m_Pos.y));

    return bytes;
}

SpawnerBuilding::SpawnerBuilding(bytedump bytes) : GenericBuilding(0) {
    assert(bytes.size() == 16);
    assert(static_cast<int>(bytes[0]) == SPAWNER_BUILDING_BYTE);

    m_HP = static_cast<int>(bytes[1]);
    m_TickCount = static_cast<int>(bytes[2]);
    m_SpawnRate = static_cast<int>(bytes[3]);

    Positionable::m_Pos = {
        static_cast<int>(bytes[14]),
        static_cast<int>(bytes[15]),
    };

    bytedump enemyBytes(bytes.begin() + 4, bytes.begin() + 11);
    m_Enemy = new Enemy(enemyBytes);
}
