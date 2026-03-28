#include "player.hpp"
#include "enemy.hpp"
#include "hand.hpp"
#include "direct_damage_spell.hpp"
#include "spell_factory.hpp"
#include "constants.hpp"

Player::Player(const std::string& name, int health, int damage, size_t hand_capacity)
    : Hero(name, health, damage), score_(0), level_(1),
      x_(START_POSITION), y_(START_POSITION), hand_(hand_capacity), kills_(0), ranged_mode_(false), slowed_(false), max_health_(health) {}

bool Player::IsRanged() const { return ranged_mode_; }
bool Player::IsSlowed() const { return slowed_; }
void Player::SetSlowed(bool value) { slowed_ = value; }


void Player::ToggleMode() {
    ranged_mode_ = (ranged_mode_ == RANGE_MODE_OFF) ? RANGE_MODE_ON : RANGE_MODE_OFF;
    if (ranged_mode_ == RANGE_MODE_ON) {
        damage_ -= DAMAGE_SHIFT;
        std::cout << "🏹 " << name_ << " переключился на дальний бой (урон: " << damage_ << ")\n";
    } else {
        damage_ += DAMAGE_SHIFT;
        std::cout << "🗡 " << name_ << " переключился на ближний бой! (урон: " << damage_ << ")\n";
    }
}

void Player::TakeDamage(int amount) {
    this->health_ -= amount;
    if (this->health_ < 0) this->health_ = 0;

    std::cout << this->name_ << " получает " << amount << " урона.\n";

    if (this->health_ == 0) {
        std::cout << "💀 " << this->name_ << " погиб!\n";
    }
}

void Player::Attack(Hero& target) {
    if (!IsAlive()) return;

    std::cout << "⚔️ " << name_ << " атакует " << target.GetName()
              << " и наносит " << damage_ << " урона!\n";

    target.TakeDamage(damage_);

    if (!target.IsAlive()) {
        AddKill();
    }
}

void Player::PickUpPotion(const Potion& potion) {
    bag_.push_back(potion);
    std::cout << name_ << " подобрал " << potion.GetName()
              << " (+" << potion.GetHealAmount() << " HP)\n";
}

void Player::UsePotion(int index) {
    if (bag_.empty()) {
        std::cout << name_ << " пытается выпить зелье, но в сумке пусто!\n";
        return;
    }

    if (index < 0 || index >= static_cast<int>(bag_.size())) {
        std::cout << "❌ Неверный номер зелья.\n";
        return;
    }

    Potion potion = bag_[index];
    bag_.erase(bag_.begin() + index);

    health_ += potion.GetHealAmount();
    if (health_ > max_health_) 
        health_ = max_health_;

    std::cout << name_ << " выпивает " << potion.GetName()
              << " и восстанавливает " << potion.GetHealAmount()
              << " HP. Теперь HP = " << health_ << "\n";
}

void Player::PrintInventory() const {
    std::cout << "\n--- Инвентарь " << name_ << " ---\n";

    if (bag_.empty()) {
        std::cout << "(Пусто)\n";
        return;
    }
    for (size_t i = 0; i < bag_.size(); ++i) {
        std::cout << i + 1 << ". " << bag_[i].GetName()
                  << " (+" << bag_[i].GetHealAmount() << " HP)\n";
    }
    std::cout << "-----------------------------\n";
}

void Player::PrintStatus() const {
    std::cout << "\n--- Статус игрока ---\n";
    std::cout << "Имя: " << name_ << "\n";
    std::cout << "HP: " << health_ << "\n";
    std::cout << "Урон: " << damage_ << "\n";
    std::cout << "Уровень: " << level_ << "\n";
    std::cout << "Очки: " << score_ << "\n";
    std::cout << "Режим боя: "
              << (ranged_mode_ ? "🏹 Дальний бой" : "🗡 Ближний бой") << "\n";
    std::cout << "----------------------\n";
}

void Player::SetPosition(int x, int y) { this->x_ = x; this->y_ = y; }
int Player::GetX() const { return x_; }
int Player::GetY() const { return y_; }

bool Player::Move(int dx, int dy, Field& field, const std::vector<Enemy>& enemies) {
    int nx = x_ + dx;
    int ny = y_ + dy;

    if (nx < 0 || ny < 0 || nx >= field.width() || ny >= field.height()) {
        std::cout << "🚫 " << name_ << " уперся в границы карты.\n";
        return MOVE_BLOCKED;
    }

    Cell& target = field.At(nx, ny);
    CellType type = target.type();

    // препятствия
    if (type == CellType::kWall ||
        type == CellType::kEnemyBuilding ||
        type == CellType::kEnemyTower ||
        type == CellType::kAlly)
    {
        std::cout << "🚧 " << name_ << " не может пройти — препятствие!\n";
        return MOVE_BLOCKED;
    }

    // враг на клетке
    for (const auto& e : enemies) {
        if (e.IsAlive() && e.GetX() == nx && e.GetY() == ny) {
            std::cout << "⚠️ " << name_
                      << " не может встать на клетку, где стоит враг (" 
                      << nx << ", " << ny << ")!\n";
            return MOVE_BLOCKED;
        }
    }

    // зелья
    if (type == CellType::kPotionSmall || type == CellType::kPotionLarge) {
        Potion potion = (type == CellType::kPotionSmall)
                            ? Potion("Малое зелье", POTION_SMALL_HEAL)
                            : Potion("Большое зелье", POTION_LARGE_HEAL);

        PickUpPotion(potion);
        target.SetType(CellType::kEmpty);
        std::cout << "💡 Чтобы использовать зелье — нажмите I.\n";
    }

    // замедляющая клетка
    if (type == CellType::kSlowCell) {
        slowed_ = true;
        skip_next_turn_ = true; // <-- главный флаг
        std::cout << "🌀 ... следующий ход пропускается\n";
    }

    // перемещение
    field.At(x_, y_).SetType(CellType::kEmpty);
    x_ = nx;
    y_ = ny;
    field.At(x_, y_).SetType(CellType::kPlayer);

    std::cout << "🚶‍♂️ " << name_ << " переместился в (" << x_ << ", " << y_ << ")\n";
    return MOVE_ALLOWED;
}

void Player::AddKill() {
    ++kills_;

    if (kills_ % 4 == 0) {
        std::cout << "🏅 " << GetName() << " уничтожил " << kills_ 
                  << " врагов и заслужил новое заклинание!\n";

        SpellFactory factory;
        auto newSpell = factory.GenerateRandomSpell();
        if (hand_.AddSpell(std::move(newSpell))) {
            std::cout << "✨ Новое заклинание добавлено в руку!\n";
        } else {
            std::cout << "🖐 Рука переполнена — заклинание не добавлено.\n";
        }
    }
}