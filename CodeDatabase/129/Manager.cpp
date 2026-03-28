#include "Manager.h"

Manager::Manager() {}
Manager::Manager(int height, int width, int hand_size, int level){
    field = Field(height, width);
    player.set_coords(MANAGER::PLAYER_COORDS_X, MANAGER::PLAYER_COORDS_Y);
    enemies.push_back(Enemy());
    moves = 0;
    hand = Hand(hand_size);
    enemy_tower = EnemyTower(Point(height/2, width/2));
    current_level = level;

    killed['c'] = 0;
    killed['r'] = 0;
    killed[CONTROL::ACT_DD_SPELL] = 0;
    killed[CONTROL::ACT_AR_SPELL] = 0;
    killed[CONTROL::ACT_TR_SPELL] = 0;
}

Manager::Manager(int height, int width, std::vector<std::string> spells, int level){
    field = Field(height, width);
    player.set_coords(MANAGER::PLAYER_COORDS_X, MANAGER::PLAYER_COORDS_Y);
    enemies.push_back(Enemy(current_level+1));
    moves = 0;
    hand = Hand(spells);
    enemy_tower = EnemyTower(Point(height/2, width/2));
    current_level = level;

    killed['c'] = 0;
    killed['r'] = 0;
    killed['d'] = 0;
    killed['a'] = 0;
    killed['t'] = 0;
}
Manager::~Manager() {}


//управление персонажем
//возвращаем - жив ли он
bool Manager::process(char c, int level){
    bool continue_game = true; //изначально жив

    moves++;
    if(moves % MANAGER::ADD_ENEMY == 0){ //каждые 11 ходов спавним врага
        enemies.push_back(Enemy(current_level + 1));
        moves = 0;
    }

    if(moves % MANAGER::ADD_SPELL == 0){
        hand.add_spell(); //каждые 20 ходов добавляем заклинание
    }

    //ход игрока
    switch(c){
        case CONTROL::SHIFT_UP: case CONTROL::SHIFT_DOWN: 
        case CONTROL::SHIFT_LEFT: case CONTROL::SHIFT_RIGHT:
            continue_game = move_player(c); //умер он или нет
            break;

        case CONTROL::HIT_UP: case CONTROL::HIT_DOWN: 
        case CONTROL::HIT_LEFT: case CONTROL::HIT_RIGHT:
            player_hit_enemy(c);
            break;

        case CONTROL::COMBAT:
            player.set_combat(!player.is_close_combat()); 
            break;
    }

    //ход врагов
    move_enemies();

    //ход вражеской башни
    if(enemy_tower.time_to_kill()){
        bool was = enemy_tower_hit_player();
        if(!was){
            enemy_tower.reset_regenerate_moves();
        } else {
            enemy_tower.set_regenerate_moves();
        }
    } else {
        enemy_tower.wait_moves();
    }

    return continue_game; //&& !check_condition(level);
}


//использование заклинания
//возвращаем - успешно ли использовали мы заклинание (ударили по врагам)
bool Manager::use_spell(char spell_c, char direct_c){
    if(!hand.can_use_spell(spell_c)) return false;

    std::unique_ptr<Spell> spell = hand.get_spell(spell_c);
    std::vector<Point> dests = spell->get_dests(player.get_coords(), direct_c);   
    bool draw = spell->draw(); //требует ли это заклинание чтоб его рисовали
    
    bool was = false;
    for(Point dest : dests){
        if(hurt_enemies(dest, spell->get_damage())){
            was = true;
        }
    }

    if(was) killed[spell_c]++; //если ударили - то обновляем статистику

    //если его нужно регенерировать при !was - регенерируем
    hand.check_spell(std::move(spell), was);
    field.set_trap_at(dests[0], draw);

    return was;
}


//для отрисовки
GField Manager::sync(){
    GField gfield(field.get_sizes().x, field.get_sizes().y);
    gfield.sync_field(field);
    return gfield;
}


//для работы
bool Manager::check_condition(int level){
    switch(level){
        case 1:
            return killed[CONDITION::WHAT_1] >= HOW_1;
        case 2:
            return killed[CONDITION::WHAT_2] >= HOW_2;
        case 3:
            return killed[CONDITION::WHAT_31] + killed[CONDITION::WHAT_32] + killed[CONDITION::WHAT_33] >= HOW_3;
    }

    return false;
}

std::vector<std::string> Manager::get_reset_spells(){
    return hand.get_reset_spells();
}


//для сохранения
//враги - отдельно
std::vector<Saving::EnemyData> Manager::enemies_serialize()const{
    std::vector<Saving::EnemyData> result;
    for(Enemy enemy : enemies){
        result.push_back(enemy.serialize());
    }
    return result;
}

void Manager::enemies_deserialize(const std::vector<Saving::EnemyData>& data){
    enemies.clear();
    for(Saving::EnemyData enemydata : data){
        Enemy enemy;
        enemy.deserialize(enemydata);
        enemies.push_back(enemy);
    }
}


//всё разом
Saving Manager::serialize() const{
    Saving::ManagerData result;
    result.level = current_level;
    result.field = field.serialize();
    result.player = player.serialize();
    result.enemies = enemies_serialize();
    result.killed = killed;
    result.hand = hand.serialize();

    Saving end_result;
    end_result.manager = result;
    return end_result;
}

void Manager::deserialize(const Saving& data){
    Saving::ManagerData end_data = data.manager;

    current_level = end_data.level;
    field.deserialize(end_data.field);
    player.deserialize(end_data.player);
    killed = end_data.killed;
    hand.deserialize(end_data.hand);
    enemies_deserialize(end_data.enemies);
}
