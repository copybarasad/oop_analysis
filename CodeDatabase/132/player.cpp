#include "player.hpp"

std::vector<Weapon> PLAYER_WEAPONS = { Sword(), Rifle() };
const int PLAYER_SPEED = 1;
const int PLAYER_HEALTH = 100;

Player::Player(): 
    MovableEntity(PLAYER_HEALTH), 
    Armed(PLAYER_WEAPONS),
    HasHand(3)
{
    screen_symbol = 'P';
}

Player::Player(PlayerImage player_image):
    MovableEntity(player_image.health_points, player_image.can_move), 
    Armed(PLAYER_WEAPONS, player_image.weapon_index),
    HasHand(player_image.hand_image)
{
    screen_symbol = 'P';
}

PlayerImage Player::get_data_image() {
    return PlayerImage(
        health_points,
        can_move,
        weapon_index,
        hand.get_data_image(),
        get_effects_img()
    );
}

// TODO: remove nearest
Move Player::choose_move(const std::vector<Position> &step_moves, const std::vector<Position> &attack_moves, const Position &cur_pos, const Position &nearest) {
    while(true) {
        char move = Control::get_move();

        if (Key::POSITIONAL.find(move) != Key::POSITIONAL.end()) {
            std::pair<int, int> move_desc = Key::DESCRIPTION[move];
            const Position cur_move = { cur_pos.x + move_desc.first, cur_pos.y + move_desc.second };

            if (std::find(step_moves.begin(), step_moves.end(), cur_move) != step_moves.end()) {
                return { cur_pos, cur_move, MoveType::MOVE };
            }

            if (std::find(attack_moves.begin(), attack_moves.end(), cur_move) != attack_moves.end()) {
                return { cur_pos, cur_move, MoveType::ATTACK };
            }
        }

        if (move == Key::CHANGE_COMBAT) {
            return { cur_pos, cur_pos, MoveType::CHANGE_WEAPON };
        }

        if (move == Key::SHOOT && get_current_weapon().combat_mode() == CombatMode::LONG_RANGE) {
            return { cur_pos, nearest, MoveType::ATTACK };
        }

        // if (move == Key::SPELL) {
        //     return { cur_pos, nearest, MoveType::ATTACK };
        // }
    }
}

GameObject* Player::clone() const {
    return new Player(*this);
}