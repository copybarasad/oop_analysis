#include "Game.h"
#include "GameException.h"
#include <iostream>

int main() {
	try {
		Game game;
		
		if (!game.showStartScreen()) {
			std::cout << "\nВыход из игры...\n";
			return 0;
		}
		
		game.startGameLoop();
		
	} catch (const FileWriteException& e) {
		std::cerr << "\n✗ Ошибка записи файла: " << e.what() << "\n";
		std::cerr << "Игра не может сохранить данные. Проверьте права доступа к файлам.\n";
		return 1;
	} catch (const FileNotFoundException& e) {
		std::cerr << "\n✗ Файл не найден: " << e.what() << "\n";
		std::cerr << "Файл сохранения не найден.\n";
		return 1;
	} catch (const CorruptedDataException& e) {
		std::cerr << "\n✗ Поврежденные данные: " << e.what() << "\n";
		std::cerr << "Файл сохранения поврежден и не может быть загружен.\n";
		return 1;
	} catch (const GameException& e) {
		std::cerr << "\n✗ Ошибка игры: " << e.what() << "\n";
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "\n✗ Неожиданная ошибка: " << e.what() << "\n";
		return 1;
	}
	
	return 0;
}
