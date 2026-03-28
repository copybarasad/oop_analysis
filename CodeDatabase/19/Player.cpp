#include "Player.h"

#include "spell/types/DamageSpell.h"
#include "world/World.h"

void Player::render(const IRenderer & renderer, const World &world, int center_x, int center_y) {
    render_model(renderer, get_attack_mode() == AttackMode::Ranged ? "player_ranged" : "player", center_x - GUI_SCALE * 3, center_y, 6, 24);
    render_health(renderer, world, center_x, center_y - GUI_SCALE * 25);
}

Player::Player(const int x, const int y) : Entity(EntityType::Player, x, y) {
    this->set_attribute(Attribute::Health, 30);
    this->set_attribute(Attribute::MaxHealth, 30);
    this->set_attribute(Attribute::Damage, 3);
    this->set_attribute(Attribute::RangedDamage, 1);
    spell_holder_.add_random_spell();
}
