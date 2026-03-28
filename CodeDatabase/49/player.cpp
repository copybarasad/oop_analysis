#include "player.h"
#include "../map/game_map.h"
#include "../logging/event_manager.h"
#include <sstream>

Player::Player(int start_x, int start_y)
    : x(start_x), y(start_y), score(0), mana(START_MANA), max_mana(MAX_MANA), health(DEFAULT_HEALTH),
      max_health(DEFAULT_HEALTH), melee_damage(MELEE_DAMAGE), range_damage(RANGE_DAMAGE),
      combat_mode(CombatMode::MELEE), slowdown_interval(SLOWDOWN_INTERVAL), slowdown_cooldown(0), spell_hand()
{
    spell_hand.initialize_hand();
}

Player::Player(const Player& other)
    : x(other.x), y(other.y), score(other.score), mana(other.mana), max_mana(other.max_mana),
      health(other.health), max_health(other.max_health), melee_damage(other.melee_damage),
      range_damage(other.range_damage), combat_mode(other.combat_mode), slowdown_interval(other.slowdown_interval), 
      slowdown_cooldown(other.slowdown_cooldown), spell_hand(other.spell_hand) {}

Player& Player::operator=(const Player& other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        score = other.score;
        mana = other.mana;
        max_mana = other.max_mana;
        health = other.health;
        max_health = other.max_health;
        melee_damage = other.melee_damage;
        range_damage = other.range_damage;
        combat_mode = other.combat_mode;
        slowdown_interval = other.slowdown_interval;
        slowdown_cooldown = other.slowdown_cooldown;
        spell_hand = other.spell_hand;
    }
    return *this;
}

int Player::get_x() const { return x; }
int Player::get_y() const { return y; }
int Player::get_score() const { return score; }
int Player::get_mana() const { return mana; }
int Player::get_max_mana() const { return max_mana; }
int Player::get_health() const { return health; }
int Player::get_max_health() const { return max_health; }
int Player::get_damage() const
{
    if (combat_mode == CombatMode::MELEE) return melee_damage;
    return range_damage;
}
CombatMode Player::get_combat_mode() const {return combat_mode; }
SpellHand& Player::get_spell_hand() { return spell_hand; }
const SpellHand& Player::get_spell_hand() const { return spell_hand; }

void Player::set_mana(int new_mana) { mana = new_mana; }
void Player::set_health(int new_health) { health = new_health; }
void Player::set_position(int new_x, int new_y)
{
    x = new_x;
    y = new_y;
}

void Player::add_score(int amount)
{
    score += amount;
}

void Player::add_mana(int amount)
{
    mana = std::min(max_mana, mana + amount);
}

void Player::subtract_mana(int amount)
{
    mana = std::max(0, mana - amount);
}

bool Player::is_alive() const
{
    return health > 0;
}

void Player::take_damage(int enemy_damage)
{
    health = std::max(0, health - enemy_damage);
    
    std::ostringstream oss;
    oss << "Player took " << enemy_damage << " damage, health: " << health << "/" << max_health;
    EventManager::get_instance().notify(GameEvent(GameEventType::PLAYER_DAMAGED, oss.str()));
}

void Player::switch_combat_mode()
{
    if (combat_mode == CombatMode::MELEE)
    {
        combat_mode = CombatMode::RANGED;
    }
    else
    {
        combat_mode = CombatMode::MELEE;
    }
    
    std::string mode_str = (combat_mode == CombatMode::MELEE) ? "MELEE" : "RANGED";
    EventManager::get_instance().notify(GameEvent(GameEventType::PLAYER_MODE_CHANGED, 
        "Player switched to " + mode_str + " mode"));
}

void Player::upgrade_max_mana()
{
    max_mana += INCREASE_MANA;
}

void Player::upgrade_max_health()
{
    max_health += INCREASE_HEALTH;
}

void Player::update()
{
    if (slowdown_cooldown > 0)
    {
        slowdown_cooldown--;
    }
}

void Player::reset_cooldown()
{
    slowdown_cooldown = slowdown_interval;
}

bool Player::can_move() const
{
    return slowdown_cooldown == 0;
}

bool Player::move(GameMap& map, int new_x, int new_y)
{
    if (!can_move()) return false;
    if (!map.is_valid_position(new_x, new_y) || !map.get_cell(new_x, new_y).get_availability()) return false;

    int old_x = x;
    int old_y = y;
    
    map.get_cell(x, y).set_type(CellType::EMPTY);

    set_position(new_x, new_y);
    map.get_cell(new_x, new_y).set_type(CellType::PLAYER);

    if (map.get_cell(new_x, new_y).get_slow_trap())
    {
        reset_cooldown();
    }

    std::ostringstream oss;
    oss << "Player moved from (" << old_x << "," << old_y << ") to (" << new_x << "," << new_y << ")";
    EventManager::get_instance().notify(GameEvent(GameEventType::PLAYER_MOVED, oss.str()));

    return true;
}

void Player::attack(GameMap& map)
{
    int player_damage = get_damage();
    std::string mode_str = (combat_mode == CombatMode::MELEE) ? "melee" : "ranged";
    int enemies_hit = 0;

    int attack_directions[4][2] =
    {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> drop_dis(0, 100);

    if (combat_mode == CombatMode::MELEE)
    {
        for (const auto& dir : attack_directions)
        {
            int attack_x = x + dir[0];
            int attack_y = y + dir[1];
            
            if (map.is_valid_position(attack_x, attack_y))
            {
                auto& enemies = map.get_enemies_mutable();
                for (size_t i = 0; i < enemies.size(); ++i)
                {
                    if (enemies[i].is_alive() && 
                        enemies[i].get_x() == attack_x && 
                        enemies[i].get_y() == attack_y)
                    {
                        enemies[i].take_damage(player_damage);
                        enemies_hit++;
                        
                        std::ostringstream oss;
                        oss << "Enemy at (" << attack_x << "," << attack_y << ") took " 
                            << player_damage << " damage, health: " << enemies[i].get_health();
                        EventManager::get_instance().notify(GameEvent(GameEventType::ENEMY_DAMAGED, oss.str()));
                        
                        if (!enemies[i].is_alive())
                        {
                            add_score(SCORE_PER_KILL);
                            add_mana(MANA_PER_KILL_MELEE);
                            
                            Cell& cell = map.get_cell(attack_x, attack_y);
                            cell.set_type(CellType::EMPTY);
                            cell.set_availability(true);
                            
                            if (drop_dis(gen) < SPELL_DROP_CHANCE && 
                                !spell_hand.is_full())
                            {
                                add_random_spell_to_hand();
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (const auto& dir : attack_directions)
        {
            for (int distance = 1; distance <= RANGED_ATTACK_RANGE; ++distance)
            {
                int attack_x = x + dir[0] * distance;
                int attack_y = y + dir[1] * distance;
                
                if (!map.is_valid_position(attack_x, attack_y)) break;
                
                Cell& cell = map.get_cell(attack_x, attack_y);
                if (cell.get_type() == CellType::WALL) break;
                
                auto& enemies = map.get_enemies_mutable();
                for (size_t i = 0; i < enemies.size(); ++i)
                {
                    if (enemies[i].is_alive() && 
                        enemies[i].get_x() == attack_x && 
                        enemies[i].get_y() == attack_y)
                    {
                        enemies[i].take_damage(player_damage);
                        enemies_hit++;
                        
                        std::ostringstream oss;
                        oss << "Enemy at (" << attack_x << "," << attack_y << ") took " 
                            << player_damage << " damage, health: " << enemies[i].get_health();
                        EventManager::get_instance().notify(GameEvent(GameEventType::ENEMY_DAMAGED, oss.str()));

                        if (!enemies[i].is_alive())
                        {
                            add_score(SCORE_PER_KILL);
                            add_mana(MANA_PER_KILL_RANGE);
                            
                            cell.set_type(CellType::EMPTY);
                            cell.set_availability(true);
                            
                            if (drop_dis(gen) < SPELL_DROP_CHANCE && 
                                !spell_hand.is_full())
                            {
                                add_random_spell_to_hand();
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    
    std::ostringstream oss;
    oss << "Player attacked (" << mode_str << "), hit " << enemies_hit << " enemies for " << player_damage << " damage each";
    EventManager::get_instance().notify(GameEvent(GameEventType::PLAYER_ATTACKED, oss.str()));
}

void Player::restore_health()
{
    health = max_health;
}

void Player::add_random_spell_to_hand()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> spell_dis(0, 1);
    
    std::unique_ptr<ISpell> new_spell;
    
    if (spell_dis(gen) == 0)
    {
        new_spell = std::make_unique<DirectDamageSpell>();
    }
    else
    {
        new_spell = std::make_unique<AreaDamageSpell>();
    }
    
    std::string spell_name = new_spell->get_name();
    get_spell_hand().add_spell(std::move(new_spell));
    
    EventManager::get_instance().notify(GameEvent(GameEventType::SPELL_RECEIVED, 
        "Player received spell: " + spell_name));
}

void Player::cast_spell(ISpell* spell, GameMap& map, int target_x, int target_y)
{
    if (spell && spell->is_available(mana))
    {
        std::ostringstream oss;
        oss << "Player cast " << spell->get_name() << " at (" << target_x << "," << target_y 
            << "), mana cost: " << spell->get_mana_cost();
        EventManager::get_instance().notify(GameEvent(GameEventType::SPELL_CAST, oss.str()));
        
        spell->cast(map, target_x, target_y);
    }
}

void Player::update_spell_cooldown(ISpell* spell)
{
    if (spell)
    {
        spell->update_cooldown();
    }
}

void Player::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&x), sizeof(x));
    os.write(reinterpret_cast<const char*>(&y), sizeof(y));
    os.write(reinterpret_cast<const char*>(&score), sizeof(score));
    os.write(reinterpret_cast<const char*>(&mana), sizeof(mana));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&melee_damage), sizeof(melee_damage));
    os.write(reinterpret_cast<const char*>(&range_damage), sizeof(range_damage));

    int32_t cm = static_cast<int32_t>(combat_mode);
    os.write(reinterpret_cast<const char*>(&cm), sizeof(cm));

    os.write(reinterpret_cast<const char*>(&slowdown_interval), sizeof(slowdown_interval));
    os.write(reinterpret_cast<const char*>(&slowdown_cooldown), sizeof(slowdown_cooldown));

    os.write(reinterpret_cast<const char*>(&max_health), sizeof(max_health));
    os.write(reinterpret_cast<const char*>(&max_mana), sizeof(max_mana));

    spell_hand.save_binary(os);
}

void Player::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&x), sizeof(x));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
    is.read(reinterpret_cast<char*>(&score), sizeof(score));
    is.read(reinterpret_cast<char*>(&mana), sizeof(mana));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&melee_damage), sizeof(melee_damage));
    is.read(reinterpret_cast<char*>(&range_damage), sizeof(range_damage));
    int32_t cm = 0;
    is.read(reinterpret_cast<char*>(&cm), sizeof(cm));
    combat_mode = static_cast<CombatMode>(cm);
    is.read(reinterpret_cast<char*>(&slowdown_interval), sizeof(slowdown_interval));
    is.read(reinterpret_cast<char*>(&slowdown_cooldown), sizeof(slowdown_cooldown));
    is.read(reinterpret_cast<char*>(&max_health), sizeof(max_health));
    is.read(reinterpret_cast<char*>(&max_mana), sizeof(max_mana));

    spell_hand.load_binary(is);
}