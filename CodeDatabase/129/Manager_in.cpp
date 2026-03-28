#include "Manager.h"

//ход врагов
void Manager::move_enemies(){
    Point player_coords = player.get_coords(); //получаем коо игрока
    for(int i = 0; i < enemies.size(); i++){
        if(!enemies[i].is_alive()) continue; //если этот враг мертв - скипаем
            
        Point source = enemies[i].get_coords(); //коо этого врага
        int dx = player_coords.x - source.x;
        int dy = player_coords.y - source.y;

        Point pre_dest;

        if(my_abs(dx) > my_abs(dy) ){
            if(dx > 0){
                if(field.can_shift_right(source)){
                    enemies[i].shift_right();
                }
            } else {
                if(field.can_shift_left(source)){
                    enemies[i].shift_left();
                }
            }
        } else {
            if(dy < 0){
                if(field.can_shift_up(source)){
                    enemies[i].shift_up();
                }
            } else {
                if(field.can_shift_down(source)){
                    enemies[i].shift_down();
                }
            }
        }

        Point destination = enemies[i].get_coords();
        bool reached_player = field.move_enemy(source, destination);
        bool reached_trap = field.is_trap_at(destination);
            
        if(reached_player) {
            player.take_damage(enemies[i].get_damage()); // враг наносит 1 урон 
            enemies[i].take_damage(enemies[i].get_lifes()); //умирает после
        }

        if(reached_trap){
            enemies[i].take_damage(enemies[i].get_lifes());
            field.set_trap_at(destination, false);
        }
    }
    check_enemies(); //проверяем кто жив а кто нет
}

//обновляем массив - удаляем мертвых врагов
void Manager::check_enemies(){
    //удаляем врага если он мертв
    for(int i = 0; i < enemies.size(); i++){
        if(!enemies[i].is_alive()){
            field.remove_enemy(enemies[i].get_coords());
            enemies.erase(enemies.begin() + i);
            i--;
        }
    }
}


//нанести врагу урон
bool Manager::hurt_enemies(Point dest, int damage){
    bool was = false;
    for(int i = 0; i < enemies.size(); i++){
        if(enemies[i].get_coords() == dest && enemies[i].is_alive()){
            enemies[i].take_damage(damage);
            was = true;
            if(!enemies[i].is_alive()){
                field.remove_enemy(dest);
            }
        }
    }

    if(player.get_coords() == dest) was = true;
    check_enemies();
    return was;
}


//игрок бьет врага
void Manager::player_hit_enemy(char c){
    Point coords = player.get_coords();
    Point direct1(0, 0); Point direct2(0,0);
    if(!player.is_close_combat()){
        switch(c){
            case CONTROL::HIT_UP:
                direct2.y -= MANAGER::RANGE_DIRECT;
                break;
            case CONTROL::HIT_DOWN:
                direct2.y += MANAGER::RANGE_DIRECT;
                break;
            case CONTROL::HIT_LEFT:
                direct2.x -= MANAGER::RANGE_DIRECT;
                break;
            case CONTROL::HIT_RIGHT:
                direct2.x += MANAGER::RANGE_DIRECT;
                break;
        }
    }
    switch(c){
        case CONTROL::HIT_UP:
            direct1.y -= MANAGER::CLOSE_DIRECT;
            break;
        case CONTROL::HIT_DOWN:
            direct1.y += MANAGER::CLOSE_DIRECT;
            break;
        case CONTROL::HIT_LEFT:
            direct1.x -= MANAGER::CLOSE_DIRECT;
            break;
        case CONTROL::HIT_RIGHT:
            direct1.x += MANAGER::CLOSE_DIRECT;
            break;
    }

    Point dest1 = Point(coords.x + direct1.x, coords.y + direct1.y);
    bool had_enemy_before = field.has_enemy_at(dest1);
    bool was1 = hurt_enemies(dest1, player.get_damage());
    bool has_enemy_after = field.has_enemy_at(dest1);


    if(player.is_close_combat() && was1){
        killed['c']++;
    }
    if(!player.is_close_combat() && had_enemy_before && !has_enemy_after){
        killed['r']++;
    }

    if(direct2.x == 0 && direct2.y == 0) return;


    Point dest2 = Point(coords.x + direct2.x, coords.y + direct2.y);
    had_enemy_before = field.has_enemy_at(dest2);
    bool was2 = hurt_enemies(dest2, player.get_damage());
    has_enemy_after = field.has_enemy_at(dest2);

    if(was2 && had_enemy_before && !has_enemy_after){
        killed['r']++;
    }
}


//игрок двигается
bool Manager::move_player(char c){
    Point source = player.get_coords();
    Point destination = source; //получаем коо игрока и устанавливаем по
                                    //умолчанию как будто он не двигается
    if(player.get_lifes() == 0){
        return false; //если игрок мертв то не двигаемся и выходим из игры
    }
    switch(c){
        case CONTROL::SHIFT_LEFT:
            if(field.can_shift_left(source)){
                player.shift_left();
            }
            break;
        case CONTROL::SHIFT_DOWN:
            if(field.can_shift_down(source)){
                player.shift_down();
            }
            break;
        case CONTROL::SHIFT_UP:
            if(field.can_shift_up(source)){
                player.shift_up();
            }
            break;
        case CONTROL::SHIFT_RIGHT:
            if(field.can_shift_right(source)){
                player.shift_right();
            }
            break;
    }

    destination = player.get_coords();
    field.move_player(source, destination);

    return true;
}


bool Manager::enemy_tower_hit_player(){
    std::vector<Point> dests = enemy_tower.get_dests();
    bool was = false;
    for(Point dest : dests){
        if(player.get_coords() == dest){
            player.take_damage(enemy_tower.get_damage());
            was = true;
        }
    }
    return was;
}


