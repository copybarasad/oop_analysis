#pragma once

class PlayingField;
class Player;

class ConsoleRenderer {
public:
    void draw(const PlayingField& field) const;

    void showPlayerStats(const Player& player, const PlayingField& field) const;
};