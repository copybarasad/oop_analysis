#include "spawnbuilding.h"

SpawnBuilding::SpawnBuilding(int x, int y) {
    coord_x = x;
    coord_y = y;
    building_position = new Point(x, y, build);

}

SpawnBuilding::~SpawnBuilding()
{
    delete building_position;
}

int SpawnBuilding::get_coord_x() const
{
    return coord_x;
}

int SpawnBuilding::get_coord_y() const
{
    return coord_y;
}

void SpawnBuilding::spawn(Enemy& comp, int x, int y)
{
    comp.spawn_enemy(x, y);
    comp.recovery_health();
}

void SpawnBuilding::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&coord_x), sizeof(coord_x));
    os.write(reinterpret_cast<const char*>(&coord_y), sizeof(coord_y));

    building_position->serialize(os);

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи SpawnBuilding в поток");
    }
}

void SpawnBuilding::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&coord_x), sizeof(coord_x));
    is.read(reinterpret_cast<char*>(&coord_y), sizeof(coord_y));

    building_position->deserialize(is);

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения SpawnBuilding из потока");
    }
}
