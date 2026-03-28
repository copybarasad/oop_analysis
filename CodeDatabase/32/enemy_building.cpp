#include "enemy_building.hpp"
#include "constants.hpp"

EnemyBuilding::EnemyBuilding(int x, int y, int spawn_delay)
    : x_(x), y_(y), spawn_delay_(spawn_delay), counter_(0), health_(BUILDING_HEALTH), destroyed_(false) {}

int EnemyBuilding::GetX() const { return this->x_; }

int EnemyBuilding::GetY() const { return this->y_; }

int EnemyBuilding::GetHealth() const { return this->health_; }

bool EnemyBuilding::IsDestroyed() const { return this->destroyed_; }

void EnemyBuilding::Tick(std::vector<Enemy>& enemies, Field& field) {
    ++this->counter_;
    if (this->counter_ < this->spawn_delay_)
        return;
    this->counter_ = 0;

    static const int dx8[DIRECTIONS_COUNT] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    static const int dy8[DIRECTIONS_COUNT] = { 0, 0, 1, -1, 1, -1, 1, -1 };

    for (int i = 0; i < DIRECTIONS_COUNT; ++i) {
        int nx = this->x_ + dx8[i];
        int ny = this->y_ + dy8[i];

        if (nx < FIELD_MIN || ny < FIELD_MIN || nx >= field.width() || ny >= field.height())
            continue;

        if (field.At(nx, ny).type() != CellType::kEmpty)
            continue;

        bool occupied = false;
        for (const auto& e : enemies) {
            if (e.IsAlive() && e.GetX() == nx && e.GetY() == ny) {
                occupied = true;
                break;
            }
        }
        if (occupied)
            continue;

        Enemy minion("Миньон", MINION_HEALTH, MINION_DAMAGE);
        minion.SetPosition(nx, ny);
        enemies.push_back(minion);

        std::cout << "🏰 Здание (" << this->x_ << "," << this->y_
                  << ") создало врага на (" << nx << "," << ny << ")\n";
        return;
    }

    std::cout << "🏰 Здание (" << this->x_ << "," << this->y_
              << ") не смогло создать врага — все клетки заняты.\n";
}

void EnemyBuilding::TakeDamage(int damage, Field& field) {
    if (destroyed_) return;

    health_ -= damage;
    if (health_ <= 0) {
        destroyed_ = true;
        std::cout << "🏰 Здание на (" << x_ << "," << y_ << ") уничтожено!\n";
        field.At(x_, y_).SetType(CellType::kEmpty);
    } else {
        std::cout << "🏰 Здание получает " << damage
                  << " урона. Осталось " << health_ << " HP.\n";
    }
}