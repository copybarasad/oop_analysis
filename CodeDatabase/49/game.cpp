#include "game.h"
#include "../logging/event_manager.h"
#include <sstream>

void Game::block_input_for_seconds(int milliseconds)
{
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));

    tcflush(STDIN_FILENO, TCIFLUSH);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void Game::stop()
{
    is_running = false;
}

bool Game::confirm_exit() const
{
    std::cout << "\nAre you sure you want to exit? All unsaved progress will be lost.\n";
    std::cout << "Press Y to exit or any other key to continue: ";
    std::string input;
    std::getline(std::cin, input);
    return (!input.empty() && (input[0] == 'Y' || input[0] == 'y'));
}

void Game::save_to_file()
{
    try
    {
        SaveManager::save_game(*this, SAVE_PATH);
        std::cout << "\n\033[1;32mGame saved!\033[0m\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "\n\033[1;31mSave failed:\033[0m " << e.what() << "\n";
    }
    block_input_for_seconds(DELAY_MS);
}

void Game::load_from_file()
{
    try
    {
        SaveManager::load_game(*this, SAVE_PATH);
        std::cout << "\n\033[1;32mGame loaded successfully!\033[0m\n";
    }
    catch (const std::exception& e)
    {
        std::cout << "\n\033[1;31mFailed to load save:\033[0m " << e.what() << "\n";
    }
    block_input_for_seconds(DELAY_MS);
}

bool Game::ask_restart() const
{
	std::cout << "\nPress R to restart or any other key to exit: ";
	std::string input;
	std::getline(std::cin, input);
	return (input[0] == 'R' || input[0] == 'r');
}

void Game::reset()
{
	map = GameMap(width, height);
	current_level = 1;
}

void Game::next_level()
{
    ++current_level;

    Player* old_player = map.get_player();

    level_up_menu(old_player);

    Player* new_player = new Player(0, 0);
    *new_player = *old_player;

    new_player->restore_health();
	new_player->get_spell_hand().remove_half_random();

    int new_width  = width  + 1 + current_level;
    int new_height = height + 1 + current_level;

    map = GameMap(new_width, new_height, current_level, new_player);

    std::ostringstream oss;
    oss << "Player advanced to level " << current_level << ", map size: " << new_width << "x" << new_height;
    EventManager::get_instance().notify(GameEvent(GameEventType::LEVEL_CHANGED, oss.str()));

    std::cout << "\n\033[1;36mEntering Level " << current_level << "...\033[0m\n";
    block_input_for_seconds(DELAY_MS);
}

void Game::level_up_menu(Player* player)
{
	bool is_upgrade_choose = false;
	while (!is_upgrade_choose)
	{
		std::ignore = system("clear");
		std::cout << "\n\033[1;33mLEVEL UP!\033[0m Choose an upgrade:\n";
		std::cout << "1. Increase max health (+1)\n";
		std::cout << "2. Increase max mana (+25)\n";
		std::cout << "3. Increase spell hand size (+1)\n";
		std::cout << "Your choice: ";

		std::string input;
		std::getline(std::cin, input);
		int choice = (input[0] - '0');

		switch (choice)
		{
		case 1:
			player->upgrade_max_health();
			std::cout << "\n\033[1;32mHealth upgraded!\033[0m\n";
			is_upgrade_choose = true;
			block_input_for_seconds(DELAY_MS);
			break;
		case 2:
			player->upgrade_max_mana();
			std::cout << "\n\033[1;32mMana upgraded!\033[0m\n";
			is_upgrade_choose = true;
			block_input_for_seconds(DELAY_MS);
			break;
		case 3:
			player->get_spell_hand().increase_max_size();
			std::cout << "\n\033[1;32mSpell hand capacity increased!\033[0m\n";
			is_upgrade_choose = true;
			block_input_for_seconds(DELAY_MS);
			break;
		default:
			std::cout << "\n\033[1;31mInvalid choice.\033[0m\n";
			block_input_for_seconds(INVALID_COMMAND_DELAY_MS);
			break;
		}
	}
}

Game::Game(std::size_t width, std::size_t height) : map(width, height), width(width), height(height), current_level(1)
{
    std::ifstream check(SAVE_PATH, std::ios::binary);
    if (check.good())
    {
        std::ignore = system("clear");
        std::cout << "Press C to continue or any other key to start new game: ";
        std::string input;
        getline(std::cin, input);
        char choice = input[0];

        if (choice == 'C' || choice == 'c')
        {
            try
            {
                SaveManager::load_game(*this, SAVE_PATH);
                std::cout << "Game loaded successfully!\n";
            }
            catch (const std::exception &e)
            {
                std::cout << "Failed to load save: " << e.what() << "\n";
                std::cout << "Starting a new game...\n";
            }
        }
        else
        {
            std::cout << "Starting a new game...\n";
        }
    }
    else
    {
        std::cout << "No save file found. Starting a new game...\n";
    }
}

void Game::save_binary(std::ostream& os) const
{
    os.write(reinterpret_cast<const char*>(&current_level), sizeof(current_level));
    uint32_t w = static_cast<uint32_t>(width);
    uint32_t h = static_cast<uint32_t>(height);
    os.write(reinterpret_cast<const char*>(&w), sizeof(w));
    os.write(reinterpret_cast<const char*>(&h), sizeof(h));

    map.save_binary(os);
}

void Game::load_binary(std::istream& is)
{
    is.read(reinterpret_cast<char*>(&current_level), sizeof(current_level));
    uint32_t w=0,h=0;
    is.read(reinterpret_cast<char*>(&w), sizeof(w));
    is.read(reinterpret_cast<char*>(&h), sizeof(h));
    width = static_cast<std::size_t>(w);
    height = static_cast<std::size_t>(h);

    map.load_binary(is);
}