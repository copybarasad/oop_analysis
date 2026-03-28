#include "game_saver.h"
#include "game.h"
#include "exceptions.h"
#include "spell_factory.h"
#include "file_manager.h"
#include <iostream>

void GameSaver::saveGame(const Game& game, const std::string& filename) {
    FileManager file(filename, std::ios::out | std::ios::trunc);

    std::fstream& stream = file.getStream();
    stream << game.current_level_ << "\n";

    stream << game.field_->getWidth() << " " << game.field_->getHeight() << "\n";

    auto player_coords = game.field_->getBattlerCoords(game.player_.get());
    stream << game.player_->getHealth() << " "
        << game.player_->getDamage() << " "
        << game.player_->getName() << "\n"
        << game.player_->getScore() << " "
        << game.player_->getDefense() << " "
        << player_coords.first << " " << player_coords.second << " "
        << game.player_->getHand().getMaxSize() << "\n";

    stream << game.player_->getHand().getSpellsCount() << "\n";
    for (const auto& spell : game.player_->getHand().getSpells()) {
        stream << static_cast<int>(spell->getType()) << " ";
    }
    stream << "\n";

    stream << game.enemies_.size() << "\n";
    for (const auto& enemy : game.enemies_) {
        auto enemy_coords = game.field_->getBattlerCoords(enemy.get());
        stream << enemy->isAlive() << " "
            << enemy->getHealth() << " "
            << enemy->getDamage() << "\n" 
            << enemy->getName() << "\n" 
            << enemy_coords.first << " " << enemy_coords.second << "\n"; 
    }
}

void GameSaver::loadGame(Game& game, const std::string& filename) {
    FileManager file(filename, std::ios::in);

    std::fstream& stream = file.getStream();

    game.resetGame();
    stream >> game.current_level_;

    int width, height;
    stream >> width >> height;

    if (stream.fail()) throw IOException("Failed to read field dimensions.");
    game.field_ = std::make_unique<PlayingField>(height, width);

    int health, damage, score, defense, x, y;
    size_t max_hand_size;
    std::string name;

    stream >> health >> damage;
    stream.ignore();
    std::getline(stream, name);

    stream >> score >> defense >> x >> y >> max_hand_size;

    if (stream.fail()) throw IOException("Failed to read player data.");

    game.player_ = std::make_unique<Player>(health, damage, name, score, defense, max_hand_size);
    game.field_->placeBattler(x, y, game.player_.get());

    size_t spell_count;
    stream >> spell_count;

    if (stream.fail()) throw IOException("Failed to read player hand data.");

    for (size_t i = 0; i < spell_count; ++i) {
        int spell_type_int;
        stream >> spell_type_int;

        if (stream.fail()) throw IOException("Failed to read spell type.");

        SpellType type = static_cast<SpellType>(spell_type_int);
        game.player_->getHand().restoreSpell(SpellFactory::createSpell(type));
    }

    size_t enemy_count;
    stream >> enemy_count;
    if (stream.fail()) throw IOException("Failed to read enemy count.");

    for (size_t i = 0; i < enemy_count; ++i) {
        bool is_alive;
        int enemy_x, enemy_y;

        stream >> is_alive >> health >> damage;
        stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::getline(stream, name);

        stream >> enemy_x >> enemy_y;
        if (stream.fail()) throw IOException("Failed to read enemy data for enemy #" + std::to_string(i));

        auto enemy = std::make_unique<Enemy>(health, damage, name);

        if (is_alive) {
            game.field_->placeBattler(enemy_x, enemy_y, enemy.get());
        }
        game.enemies_.push_back(std::move(enemy));
    }
}