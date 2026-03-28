#ifndef SETUP_HPP_INCLUDED
#define SETUP_HPP_INCLUDED

#include <vector>
#include <string>
#include "position.hpp"
#include "field.hpp"
#include "hero.hpp"
#include "spawner.hpp"

class GameParams {
private:
    int width_{12};
    int height_{12};
public:
    int getWidth() const noexcept { return width_; }
    void setWidth(int v) noexcept { width_ = v; }
    int getHeight() const noexcept { return height_; }
    void setHeight(int v) noexcept { height_ = v; }

    // герой
    Position heroStart{0,0};
    int heroHp{30};
    int heroDmg{6};

    // размер руки (лимит карт)
    int handSize{3};

    // спавн
    int spawnPeriod{3}; // раз в N ходов
    std::vector<Position> spawners;

    // клетки
    std::vector<Position> walls;
    std::vector<Position> slows;
};

/**
 * интерактивно спрашивает параметры игры у пользователя.
 * ввод координат: по одной паре "x y" на строку; окончание — пустая строка или "done".
 */
GameParams prompt_game_params();

/**
 * вспомогательная функция для трима строки
 */
std::string trim_copy(std::string s);

/**
 * применяет параметры к миру: создаёт поле, героя, спавнеры и проставляет клетки.
 * возвращает true/false при успехе/ошибке (ошибка — конфликт позиций после валидации).
 */
bool build_world_from_params(const GameParams& p,
                             Field& outField,
                             Hero& outHero,
                             std::vector<Spawner>& outSpawners);

#endif // SETUP_HPP_INCLUDED
