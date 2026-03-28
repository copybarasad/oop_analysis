#include "GameLogic.h"

#include <algorithm>
#include <format>

#include "Game.h"
#include "entity/types/Ally.h"
#include "entity/types/Enemy.h"
#include "entity/types/EnemyHouse.h"
#include "entity/types/EnemyTower.h"
#include "entity/types/Trap.h"
#include "spell/Spell.h"
#include "upgrade/PlayerUpgradeRegistry.h"
#include "upgrade/types/PlayerUpgradeGold.h"


void GameLogic::init_level() {
    LevelConfiguration configuration = get_level_config(wave_);

    delete game_.world_;

    game_.world_ = new World(configuration.world_width(), configuration.world_height());
    game_.world_->add_entity(game_.player_);
    for (auto [type, count]: configuration.monsters()) {
        for (int i = 0; i < count; ++i) {
            auto pos = game_.world().pick_random_safe_pos();
            if (pos.first < 0 || pos.second < 0) continue;

            Entity *entity = nullptr;

            switch (type) {
                case EntityType::Enemy:
                    entity = new Enemy(pos.first, pos.second);
                    break;
                case EntityType::EnemyHouse:
                    entity = new EnemyHouse(pos.first, pos.second, 6);
                    break;
                case EntityType::EnemyTower:
                    entity = new EnemyTower(pos.first, pos.second, 3);
                    break;
                case EntityType::Ally:
                    entity = new Ally(pos.first, pos.second);
                    break;
                case EntityType::Trap:
                    entity = new Trap(pos.first, pos.second, true, 5);
                    break;
                default:
                    break;
            }
            if (entity != nullptr) {
                entity->set_attribute(Attribute::Intelligence, configuration.monster_intelligence());
                add_entity(entity);
            }
        }
    }
    game_.world_->randomize_tiles(configuration.tiles());
}


void GameLogic::update(float dt) {
    since_last_action_ += dt;
    for (auto &item: game_.world().entities()) {
        item->update(*game_.world_, dt);
    }
    if (game_.state() == GameState::AIMove) {
        const auto entities = game_.world().entities();
        for (auto &entity: entities) {
            if (entity->type() == EntityType::Player) continue;
            entity->blocked_moves() > 0
                ? entity->skip_blocked_move()
                : entity->tick(*this);
        }

        int hostile = 0;
        for (const auto e: game_.world().entities()) {
            if (e->is_hostile()) {
                hostile++;
            }
        }

        for (const auto e: to_delete_) {
            delete e;
        }
        to_delete_.clear();

        if (game_.state() != GameState::Finished) {
            if (hostile == 0) {
                if (wave_ == 5) {
                    game_.state_ = GameState::Finished;
                    game_.player_won_ = true;
                    std::cout << "You won! Score: " << game_.player().score() << std::endl;
                } else {
                    set_wave(wave_ + 1);
                    clear_purchased_upgrades();
                    roll_available_upgrades();
                    game_.state_ = GameState::Upgrading;
                    game_.hud_.reload_buttons();
                }
            } else {
                game_.state_ = GameState::PlayerMove;
            }
        }
    }
}


LevelConfiguration GameLogic::get_level_config(int level) const {
    level = std::clamp(level, 1, 10);

    int width = level * 2 + 15;
    int height = level * 2 + 15;

    float current_area = static_cast<float>(width * height);
    float base_area = 14.0f * 14.0f;
    float density_scale = current_area / base_area;

    std::unordered_map<TileType, int> tile_distribution = {
            {TileType::Mud, static_cast<int>(6 * density_scale)},
            {TileType::Lava, static_cast<int>(10 * density_scale)},
            {TileType::Ice, static_cast<int>(5 * density_scale)},
    };

    std::unordered_map<EntityType, int> monsters = {
            {EntityType::Enemy, static_cast<int>(4 * density_scale)},
            {EntityType::EnemyHouse, std::max(1, static_cast<int>(1.5f * density_scale))},
            {EntityType::EnemyTower, std::max(1, static_cast<int>(1.2f * density_scale))},
    };

    int intelligence = 10 + level * 5;

    return LevelConfiguration(width, height, tile_distribution, intelligence, monsters);
}

void GameLogic::damage_entity(Entity *entity, int damage) {
    if (damage < 0) return;
    game_.sound().play("hurt", 0);
    entity->on_hurt();
    particle_helper_.spawn_blood_effect(entity->x(), entity->y());
    int hp = entity->get_attribute(Attribute::Health);
    hp -= damage;

    game_.event_handler().notify({"entity_damage",
        (std::ostringstream() << *entity << " took " << damage << " damage.").str()});

    if (hp <= 0) {
        particle_helper_.spawn_death_effect(entity->x(), entity->y());
        game_.sound().play("death", 0);
        game_.world_->remove_entity(entity);

        game_.event_handler().notify({"entity_death",
            (std::ostringstream() << *entity << " died.").str()});

        if (!entity->is_ally(game_.player())) {
            game_.player().inc_score(1);
            game_.player().get_spell_holder().add_random_spell();
            int gold = entity->get_attribute(Attribute::Gold) + get_upgrade_level("gold_upgrade") *
                                                                GOLD_BONUS;
            game_.player().add_attribute(Attribute::Gold,gold);
            game_.event_handler().notify({"receive_gold",
                                          (std::ostringstream() << "Player received" << gold << " gold.").str()});
        }
        if (entity->type() == EntityType::Player) {
            game_.state_ = GameState::Finished;
            std::cout << "You lost! Score: " << game_.player().score() << std::endl;
        } else {
            to_delete_.push_back(entity);
        }
    } else {
        entity->set_attribute(Attribute::Health, hp);
    }
}

bool GameLogic::add_entity(Entity *entity) {
    if (entity->type() == EntityType::Player) {
        if (auto *p = dynamic_cast<Player *>(entity)) {
            game_.player_ = p;
        } else {
            std::cerr << "Entity type is player but it is not a player.\n";
        }
    }
    if (game_.world_->add_entity(entity)) {
        game_.event_handler().notify({"entity_add",
            (std::ostringstream() << "Added " << *entity << ".").str()});
        return true;
    }
    return false;
}

void GameLogic::handle_player_click(int x, int y) {
    if (game_.state() != GameState::PlayerMove || since_last_action_ <= PLAYER_ACTION_DELAY) return;
    game_.event_handler().notify({"player_click",
        (std::ostringstream() << "Player clicked at (" << x << ", " << y << ").").str()});

    Player &player = game_.player();
    bool success = false;

    if (player.blocked_moves() > 0) {
        player.skip_blocked_move();
        success = true;
    } else {
        const auto &world = game_.world();

        int tile_x = world.get_tile_x(x, y);
        int tile_y = world.get_tile_y(x, y);


        if (!world.is_valid_tile(tile_x, tile_y)) {
            return;
        }


        SpellHolder &spell_holder = player.get_spell_holder();

        Spell *selected_spell = spell_holder.get_selected();

        if (spell_focus_ && selected_spell != nullptr) {
            int buff = spell_holder.get_spell_buff();

            success = selected_spell->cast(*this, game_.player(), x, y, get_next_spell_level());
            if (success) {
                game_.event_handler().notify({"cast_spell",
                                              (std::ostringstream() << "Player casted spell " << selected_spell->id() << ".").str()});
                if (spell_holder.get_spell_buff() == buff) {
                    spell_holder.set_spell_buff(0);
                }
                player.get_spell_holder().remove_spell(spell_holder.get_selected_index());
                spell_focus_ = false;
            }
        } else {
            if ((tile_x == player.x() && tile_y == player.y())) {
                return;
            }
            int manh = player.manh_distance(tile_x, tile_y);
            int cheb = player.cheb_distance(tile_x, tile_y);


            const auto &tile = world.get_tile(tile_x, tile_y);

            if (tile.occupied() && player.get_attack_mode() == AttackMode::Ranged && cheb < 3) {
                damage_entity(tile.entity(), player.get_attribute(Attribute::RangedDamage));
                particle_helper().spawn_projectile("p_stone", 0.15, 15, player.x() - 0.3, player.y() - 0.3, tile_x, tile_y);
                success = true;
            }
            else if (manh == 1) {
                if (tile.occupied() && tile.entity()->is_hostile()) {
                    int damage = player.get_attribute(Attribute::Damage);
                    damage_entity(tile.entity(), damage);
                    success = true;
                } else if (move_entity(&player, tile_x, tile_y)) {
                    success = true;
                }
            }
        }
    }
    if (success && game_.state() != GameState::Finished) {
        game_.state_ = GameState::AIMove;
        game_.sound().play("action", 0);
        since_last_action_ = 0;
    }
}

void GameLogic::toggle_spell_focus() {
    spell_focus_ = !spell_focus_;
    SpellHolder &holder = game_.player().get_spell_holder();
    if (holder.get_selected() == nullptr) {
        holder.set_selected(0);
    }
    game_.sound().play("action", 0);
}

void GameLogic::handle_next_spell() const {
    const SpellHolder &holder = game_.player().get_spell_holder();
    const int size = holder.get_spells().size();
    if (size <= 1) return;
    handle_select_spell((holder.get_selected_index() + 1) % size);
}

void GameLogic::handle_prev_spell() const {
    const SpellHolder &holder = game_.player().get_spell_holder();
    const int size = holder.get_spells().size();
    if (size <= 1) return;
    handle_select_spell((holder.get_selected_index() - 1 + size) % size);
}

void GameLogic::handle_select_spell(const int index) const {
    if (game_.state_ != GameState::PlayerMove || !spell_focus_) return;
    SpellHolder &holder = game_.player().get_spell_holder();

    if (holder.set_selected(index)) {
        game_.sound().play("action", 0);
    }
}

void GameLogic::switch_attack_mode() {
    if (game_.state() == GameState::PlayerMove && since_last_action_ > PLAYER_ACTION_DELAY) {
        since_last_action_ = 0;
        game_.player_->switch_attack_mode();
        game_.event_handler().notify({"switch_attach_mode",
        (std::ostringstream() << "Player switched attack mode to " << nlohmann::json(player().get_attack_mode()).get<std::string>() << ".").str()});

        AttackMode mode = game_.player_->get_attack_mode();


        std::string attack_mode_name;
        if (mode == AttackMode::Melee) {
            attack_mode_name = "Melee";
        } else if (mode == AttackMode::Ranged) {
            attack_mode_name = "Ranged";
        } else {
            attack_mode_name = "Unknown";
        }

        std::cout << "Attack mode is now " << attack_mode_name << std::endl;
        game_.sound().play("action", 0);

        game_.state_ = GameState::AIMove;
    }
}

const World &GameLogic::world() const {
    return game_.world();
}

Player &GameLogic::player() const {
    return game_.player();
}

void GameLogic::roll_available_upgrades() {
    const auto &possible = PlayerUpgradeRegistry::instance().ids();
    int rolling = std::min(3, static_cast<int>(possible.size()));

    current_available_upgrades_.clear();


    std::vector<std::string> shuffled = possible;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), rng);

    for (int i = 0; i < rolling; ++i) {
        current_available_upgrades_.push_back(shuffled[i]);
    }
}

int GameLogic::get_upgrade_level(const std::string &id) {
    const auto it = upgrades_.find(id);
    if (it != upgrades_.end()) {
        return it->second;
    }
    return 0;
}

void GameLogic::set_upgrade_level(const std::string &id, const int level) {
    upgrades_[id] = level;
}


bool GameLogic::move_entity(Entity *entity, int target_x, int target_y) {
    int old_x = entity->x();
    int old_y = entity->y();

    const World &const_world = *game_.world_;

    const auto &tile = const_world.get_tile(target_x, target_y);

    if (tile.entity() != nullptr) {
        Entity *target_entity = tile.entity();
        if (target_entity->type() == EntityType::Trap) {
            damage_entity(entity, target_entity->get_attribute(Attribute::Damage));
            game_.world_->remove_entity(target_entity);
            to_delete_.push_back(target_entity);


            if (!game_.world_->has_entity(entity)) {
                return false;
            }
        }
    }

    if (game_.world_->move_entity(*entity, target_x, target_y)) {
        TileType tile_type = tile.type();
        if (tile_type == TileType::Ice) {
            int dx = target_x - old_x;
            int dy = target_y - old_y;

            int slipped_x = target_x + dx;
            int slipped_y = target_y + dy;

            if (game_.world_->is_valid_tile(slipped_x, slipped_y)) {
                const auto &slipped_tile = const_world.get_tile(slipped_x, slipped_y);
                if (slipped_tile.type() == TileType::Lava) {
                    damage_entity(entity, 1);
                } else if (game_.world_->move_entity(*entity, slipped_x, slipped_y)) {
                    tile_type = slipped_tile.type();
                }
            }
        }

        if (tile_type == TileType::Mud) {
            entity->block_moves(1);
        }

        game_.event_handler().notify({"entity_move",
        (std::ostringstream() << *entity << " moved from (" << old_x << ", " << old_y << ").").str()});

        return true;
    }
    return false;
}

void GameLogic::set_wave(int wave) {
    if (wave < 1) wave = 1;
    wave_ = wave;

    auto entities = game_.world().entities();

    for (const auto e: entities) {
        game_.world_->remove_entity(e);
        if (e->type() != EntityType::Player) {
            to_delete_.push_back(e);
        }
    }

    game_.player().set_attribute(Attribute::Health, game_.player().get_attribute(Attribute::MaxHealth));
    if (wave == 1) {
        to_delete_.push_back(game_.player_);
        game_.player_ = new Player(0, 0);
    } else {
        game_.player().get_spell_holder().remove_half_spells();
    }


    init_level();

    game_.event_handler().notify({"wave_change",
        (std::ostringstream() << "Wave changed to " << wave).str()});
}

GameLogic::~GameLogic() {
    for (auto *e: to_delete_) {
        delete e;
    }
    to_delete_.clear();
}
