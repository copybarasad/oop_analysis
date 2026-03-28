#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "Game.h"
#include "GameRenderer.h"
#include "ControlConfig.h"
#include "ControlConfigLoader.h"
#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <sstream>

// Forward declaration
template<typename CommandReaderType> class GameController;

// Шаблонный класс управления игрой
// Параметр шаблона CommandReaderType - класс, отвечающий за считывание и преобразование ввода
// Класс CommandReaderType должен иметь метод readInputLine(const std::string& prompt = "")
template<typename CommandReaderType>
class GameController {
private:
	std::unique_ptr<CommandReaderType> commandReader;
	Game* game;
	GameRenderer renderer;
	ControlConfig controlConfig;  // Конфигурация управления
	
	// Обработка команды движения
	void handleMoveCommand(char direction);
	
	// Обработка команды атаки
	void handleAttackCommand();
	
	// Получение направления атаки от пользователя
	char getAttackDirection();
	
	// Обработка команды заклинания
	void handleCastSpellCommand();
	
	// Обработка команды показа руки
	void handleShowHandCommand();
	
	// Обработка команды переключения режима боя
	void handleSwitchCombatModeCommand();
	
	// Обработка команды сохранения
	void handleSaveCommand();
	
	// Обработка команды загрузки
	void handleLoadCommand();
	
	// Обработка команды выхода
	bool handleExitCommand();
	
	// Получение команды как строки от читателя
	std::string getCommandString();

public:
	// Конструктор - принимает Game, НЕ создает его
	// Опционально принимает имя файла конфигурации управления
	GameController(Game* gameInstance, const std::string& configFilename = "controls.txt");
	
	// Запуск игрового цикла
	void run();
	
	// Обработка одной итерации (получение команды и выполнение)
	bool processCommand();
};

// Реализация шаблона должна быть в .h файле

template<typename CommandReaderType>
GameController<CommandReaderType>::GameController(Game* gameInstance, const std::string& configFilename)
	: game(gameInstance), renderer() {
	if (!game) {
		throw std::invalid_argument("Game instance cannot be null in GameController constructor");
	}
	commandReader = std::make_unique<CommandReaderType>();
	
	// Загружаем конфигурацию управления из файла
	// Если файл не найден или конфигурация невалидна, используется конфигурация по умолчанию
	controlConfig = ControlConfigLoader::loadFromFile(configFilename);
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::run() {
	if (!game) {
		return;
	}
	
	renderer.renderWelcomeMessage();
	renderer.renderGameControls(controlConfig);  // Используем конфигурацию для отображения
	renderer.renderGameLegend();
	
	bool continuePlaying = true;
	while (continuePlaying) {
		game->startLevel();
		
		// Основной игровой цикл уровня
		while (game->getGameState() == GameState::RUNNING) {
			renderer.renderFullGameState(*game);
			
			if (!processCommand()) {
				// Команда выхода
				return;
			}
		}
		
		// Обработка окончания уровня/игры
		continuePlaying = game->handleGameOver();
	}
}

template<typename CommandReaderType>
bool GameController<CommandReaderType>::processCommand() {
	if (!game || !commandReader) {
		return false;
	}
	
	// Получаем команду от читателя как строку
	std::string command = getCommandString();
	
	if (command.empty()) {
		return true; // Продолжаем цикл
	}
	
	// Обрабатываем команду в зависимости от символа
	char cmd = command[0];
	
	// Получаем тип команды из конфигурации
	ControlConfig::CommandType commandType = controlConfig.getCommandForKey(cmd);
	
	switch (commandType) {
		case ControlConfig::CommandType::MOVE_UP:
			handleMoveCommand('w'); // Внутренне Game использует стандартные команды w/a/s/d
			break;
		case ControlConfig::CommandType::MOVE_DOWN:
			handleMoveCommand('s');
			break;
		case ControlConfig::CommandType::MOVE_LEFT:
			handleMoveCommand('a');
			break;
		case ControlConfig::CommandType::MOVE_RIGHT:
			handleMoveCommand('d');
			break;
		case ControlConfig::CommandType::ATTACK:
			handleAttackCommand();
			break;
		case ControlConfig::CommandType::CAST_SPELL:
			handleCastSpellCommand();
			break;
		case ControlConfig::CommandType::SHOW_HAND:
			handleShowHandCommand();
			break;
		case ControlConfig::CommandType::SWITCH_MODE:
			handleSwitchCombatModeCommand();
			break;
		case ControlConfig::CommandType::SAVE:
			handleSaveCommand();
			break;
		case ControlConfig::CommandType::LOAD:
			handleLoadCommand();
			break;
		case ControlConfig::CommandType::EXIT:
			return handleExitCommand();
		default:
			// Неизвестная команда - просто продолжаем
			break;
	}
	
	return true;
}

template<typename CommandReaderType>
std::string GameController<CommandReaderType>::getCommandString() {
	if (!commandReader) {
		return "";
	}
	
	// Формируем подсказку на основе текущей конфигурации
	std::ostringstream prompt;
	prompt << "Enter command (";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_UP) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_LEFT) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_DOWN) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_RIGHT) << " move, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::ATTACK) << " attack, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::CAST_SPELL) << " cast spell, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::SHOW_HAND) << " show hand, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::SWITCH_MODE) << " switch mode, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::SAVE) << " save, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::LOAD) << " load, ";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::EXIT) << " exit): ";
	
	// Используем метод readInputLine для получения команды
	std::string input = commandReader->readInputLine(prompt.str());
	
	// Приводим к нижнему регистру
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	
	// Убираем пробелы
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	
	return input;
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleMoveCommand(char direction) {
	if (!game) return;
	game->processTurn(direction);
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleAttackCommand() {
	if (!game || !commandReader) return;
	
	// Запрашиваем направление атаки с правильными клавишами из конфигурации
	char direction = getAttackDirection();
	if (direction == '\0') {
		// Пользователь отменил или ввел неверное направление
		return;
	}
	
	// Преобразуем направление из пользовательской клавиши во внутреннюю команду
	// Внутренне Game использует w/a/s/d для направлений
	char internalDirection = 'w'; // по умолчанию
	ControlConfig::CommandType dirCommand = controlConfig.getCommandForKey(direction);
	
	switch (dirCommand) {
		case ControlConfig::CommandType::MOVE_UP:
			internalDirection = 'w';
			break;
		case ControlConfig::CommandType::MOVE_DOWN:
			internalDirection = 's';
			break;
		case ControlConfig::CommandType::MOVE_LEFT:
			internalDirection = 'a';
			break;
		case ControlConfig::CommandType::MOVE_RIGHT:
			internalDirection = 'd';
			break;
		default:
			renderer.renderMessage("Invalid direction for attack.");
			return;
	}
	
	// Используем новый метод processTurnWithAttackDirection для обработки атаки с направлением
	// Это позволит использовать правильные клавиши из конфигурации
	game->processTurnWithAttackDirection('f', internalDirection);
}

template<typename CommandReaderType>
char GameController<CommandReaderType>::getAttackDirection() {
	if (!commandReader) return '\0';
	
	// Формируем подсказку с правильными клавишами из конфигурации
	std::ostringstream prompt;
	prompt << "Attack direction (";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_UP) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_LEFT) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_DOWN) << "/";
	prompt << controlConfig.getKeyForCommand(ControlConfig::CommandType::MOVE_RIGHT) << "): ";
	
	std::string input = commandReader->readInputLine(prompt.str());
	if (input.empty()) {
		return '\0';
	}
	
	// Приводим к нижнему регистру и берем первый символ
	std::transform(input.begin(), input.end(), input.begin(), ::tolower);
	return input[0];
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleCastSpellCommand() {
	if (!game) return;
	game->processTurn('c'); // c - команда заклинания
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleShowHandCommand() {
	if (!game) return;
	game->processTurn('h'); // h - команда показа руки
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleSwitchCombatModeCommand() {
	if (!game) return;
	game->processTurn('q'); // q - команда переключения режима
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleSaveCommand() {
	if (!game) return;
	
	std::string filename = commandReader->readInputLine("Enter save filename (default: savegame.txt): ");
	if (filename.empty()) {
		filename = "savegame.txt";
	}
	game->saveGame(filename);
}

template<typename CommandReaderType>
void GameController<CommandReaderType>::handleLoadCommand() {
	if (!game) return;
	
	std::string filename = commandReader->readInputLine("Enter load filename (default: savegame.txt): ");
	if (filename.empty()) {
		filename = "savegame.txt";
	}
	game->loadGame(filename);
}

template<typename CommandReaderType>
bool GameController<CommandReaderType>::handleExitCommand() {
	// Выход из игры - возвращаем false чтобы выйти из цикла
	return false;
}

#endif // GAME_CONTROLLER_H

