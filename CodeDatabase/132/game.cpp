#include "game.hpp"

Game::Game():
    field_handler(lvl_gen.get_lvl(lvl)),
    action_handler(field_handler)
{}

Game::Game(GameImage& img):
    score(img.score),
    coins(img.coins),
    time(img.time),
    lvl(img.lvl),
    cap_lvl(img.cap_lvl),
    hp_lvl(img.hp_lvl),
    field_handler(img.field_img),
    action_handler(field_handler)
{}

GameImage Game::get_data_image() {
    return GameImage(
        score,
        coins,
        time,
        lvl,
        cap_lvl,
        hp_lvl,
        field_handler.get_data_image()
    );
}

bool Game::handle_menu_action() {
    char ac = Control::get_menu_action_in_game();

    if (Menu::AVAILABLE.find(ac) != Menu::AVAILABLE.end()) {
        return true;
    }

    return false;
}

void Game::player_move(const Position& pos) {
    action_handler.handle_object_action(
        pos, 
        {'E', 'B', 'T'}, 
        {'.', '~', '_', '!'}
    );
}

void Game::ally_move(const Position& pos) {
    action_handler.handle_object_action(
        pos, 
        {'E', 'B', 'T'}, 
        {'.', '~', '_', '!'}
    );
}

void Game::enemy_move(const Position& pos) {
    action_handler.handle_object_action(
        pos, 
        {'P', 'A'}, 
        {'.', '~', '_', '!'}
    );
}

void Game::enemy_tower_move(const Position& pos) {
    action_handler.handle_object_action(
        pos, 
        {'P', 'A'}, 
        {}
    );
}

void Game::enemy_spawner_move(const Position& pos) {
    action_handler.handle_object_action(
        pos, 
        {}, 
        {'.', '~'}
    );
}

int Game::open_store() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::shared_ptr<ISpell>> spells = {
        std::make_shared<ZapSpell>(),
        std::make_shared<FireballSpell>(),
        std::make_shared<TrapSpell>(),
        std::make_shared<SummonSpell>()
    };

    if (coins >= 10) {
        Position player_pos = *field_handler.get_positions('P').begin();

        if (IHasHand* player = dynamic_cast<IHasHand*>(field_handler.get_obj(player_pos))) {
            auto& spell = spells[gen() % spells.size()];

            if (player->add_spell(spell)) {
                return 10;
            }
        }
    }

    return 0;
}

void Game::tick() {
    coins -= open_store();

    std::set<Position> p_pos_copy;
    for (auto &x: field_handler.get_positions('P')) p_pos_copy.insert(x);
    for (const Position &pos: p_pos_copy) {
        player_move(pos);
    }

    std::set<Position> a_pos_copy;
    for (auto &x: field_handler.get_positions('A')) a_pos_copy.insert(x);
    for (const Position &pos: a_pos_copy) {
        ally_move(pos);
    }

    std::set<Position> en_pos_copy;
    for (auto &x: field_handler.get_positions('E')) en_pos_copy.insert(x);
    for (const Position &pos: en_pos_copy) {
        enemy_move(pos);
    }

    std::set<Position> enb_pos_copy;
    for (auto &x: field_handler.get_positions('B')) enb_pos_copy.insert(x);
    for (const Position &pos: enb_pos_copy) {
        enemy_spawner_move(pos);
    }

    std::set<Position> ent_pos_copy;
    for (auto &x: field_handler.get_positions('T')) ent_pos_copy.insert(x);
    for (const Position &pos: ent_pos_copy) {
        enemy_tower_move(pos);
    }

    coins += 2;
    time++;
}

GameState Game::game_state() {
    if (field_handler.count_positions('P') != 0) {
        if (
            field_handler.count_positions('E') == 0 && 
            field_handler.count_positions('B') == 0 &&
            field_handler.count_positions('T') == 0
        ) {
            return GameState::VICTORY;
        }
        else {
            return GameState::IN_PROCESS;
        }
    }
    else {
        return GameState::DEFEAT;
    }
}

void Game::restart_lvl() {
    field_handler = lvl_gen.get_lvl(lvl);

    Player* player = field_handler.get_player();
    PlayerImage img = player->get_data_image();
    
    img.hand_image.capacity += 2 * cap_lvl;
    img.health_points += 20 * hp_lvl;

    player = new Player(img);
}

void Game::next_lvl() {
    Player* player = field_handler.get_player();
    HandImage img = player->get_data_image().hand_image;
    img.remove_half();

    lvl++;
    field_handler = lvl_gen.get_lvl(lvl);

    Player* new_player = field_handler.get_player();
    PlayerImage new_img = new_player->get_data_image();

    char enh = Control::get_ench();
    if (enh == Menu::CP_UP)
        cap_lvl++;
    else
        hp_lvl++;
    
    new_img.hand_image.capacity += 2 * cap_lvl;
    new_img.health_points += 20 * hp_lvl;

    new_player = new Player(new_img);
}

GameState Game::start() {
    Screen::frame(field_handler.get_field());

    while (game_state() == GameState::IN_PROCESS) {
        tick();
        Screen::frame(field_handler.get_field());

        if(handle_menu_action()) {
            return game_state();
        }
    }

    return game_state();
}