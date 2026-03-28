#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "Game.h"
#include <memory>
#include <string>
#include <utility>

// Шаблонный класс визуализации игры
// Параметр шаблона RendererType - класс, отвечающий за способ отрисовки игры
// Класс RendererType должен иметь методы:
//   - renderFullGameState(const Game& game) const
//   - renderWelcomeMessage() const (опционально)
template<typename RendererType>
class GameVisualizer {
private:
	std::unique_ptr<RendererType> renderer;
	Game* game;
	
	// Состояние для отслеживания изменений
	int lastTurnNumber;
	int lastPlayerX;
	int lastPlayerY;
	int lastPlayerHealth;
	GameState lastGameState;
	
	// Проверка, изменилось ли состояние игры
	bool hasStateChanged() const;
	
	// Обновление сохраненного состояния
	void updateSavedState();

public:
	// Конструктор - принимает Game, НЕ создает его
	// Параметр args... передается в конструктор RendererType
	template<typename... Args>
	GameVisualizer(Game* gameInstance, Args&&... args);
	
	// Конструктор по умолчанию (для рендереров без параметров)
	GameVisualizer(Game* gameInstance);
	
	// Инициализация визуализации (вызывается при старте игры)
	void initialize();
	
	// Обновление визуализации (вызывается после изменений в игре)
	void update();
	
	// Принудительная отрисовка текущего состояния
	void render();
	
	// Отрисовка при старте игры
	void renderOnGameStart();
	
	// Отрисовка при старте уровня
	void renderOnLevelStart();
	
	// Отрисовка при окончании игры
	void renderOnGameEnd();
};

// Реализация шаблона

template<typename RendererType>
template<typename... Args>
GameVisualizer<RendererType>::GameVisualizer(Game* gameInstance, Args&&... args)
	: game(gameInstance), lastTurnNumber(-1), lastPlayerX(-1), 
	  lastPlayerY(-1), lastPlayerHealth(-1), lastGameState(GameState::RUNNING) {
	if (!game) {
		throw std::invalid_argument("Game instance cannot be null in GameVisualizer constructor");
	}
	renderer = std::make_unique<RendererType>(std::forward<Args>(args)...);
}

template<typename RendererType>
GameVisualizer<RendererType>::GameVisualizer(Game* gameInstance)
	: game(gameInstance), lastTurnNumber(-1), lastPlayerX(-1), 
	  lastPlayerY(-1), lastPlayerHealth(-1), lastGameState(GameState::RUNNING) {
	if (!game) {
		throw std::invalid_argument("Game instance cannot be null in GameVisualizer constructor");
	}
	renderer = std::make_unique<RendererType>();
}

template<typename RendererType>
void GameVisualizer<RendererType>::initialize() {
	if (!game || !renderer) {
		return;
	}
	
	// Инициализируем сохраненное состояние
	updateSavedState();
	
	// Отрисовываем приветственное сообщение, если есть такой метод
	renderOnGameStart();
}

template<typename RendererType>
void GameVisualizer<RendererType>::update() {
	if (!game || !renderer) {
		return;
	}
	
	// Проверяем, изменилось ли состояние
	if (hasStateChanged()) {
		// Вызываем отрисовку
		render();
		// Обновляем сохраненное состояние
		updateSavedState();
	}
}

template<typename RendererType>
void GameVisualizer<RendererType>::render() {
	if (!game || !renderer) {
		return;
	}
	
	// Вызываем отрисовку полного состояния игры
	renderer->renderFullGameState(*game);
}

template<typename RendererType>
void GameVisualizer<RendererType>::renderOnGameStart() {
	if (!renderer) {
		return;
	}
	
	// Пытаемся вызвать renderWelcomeMessage, если он есть
	// (это опциональный метод, не все рендереры могут его иметь)
	renderer->renderWelcomeMessage();
}

template<typename RendererType>
void GameVisualizer<RendererType>::renderOnLevelStart() {
	if (!game || !renderer) {
		return;
	}
	
	// Отрисовываем состояние при старте уровня
	render();
	updateSavedState();
}

template<typename RendererType>
void GameVisualizer<RendererType>::renderOnGameEnd() {
	if (!game || !renderer) {
		return;
	}
	
	// Отрисовываем финальное состояние
	render();
}

template<typename RendererType>
bool GameVisualizer<RendererType>::hasStateChanged() const {
	if (!game) {
		return false;
	}
	
	const Player& player = game->getPlayer();
	int currentTurn = game->getTurnNumber();
	int currentX = player.getPositionX();
	int currentY = player.getPositionY();
	int currentHealth = player.getHealth();
	GameState currentState = game->getGameState();
	
	// Проверяем изменения
	return (currentTurn != lastTurnNumber ||
	        currentX != lastPlayerX ||
	        currentY != lastPlayerY ||
	        currentHealth != lastPlayerHealth ||
	        currentState != lastGameState);
}

template<typename RendererType>
void GameVisualizer<RendererType>::updateSavedState() {
	if (!game) {
		return;
	}
	
	const Player& player = game->getPlayer();
	lastTurnNumber = game->getTurnNumber();
	lastPlayerX = player.getPositionX();
	lastPlayerY = player.getPositionY();
	lastPlayerHealth = player.getHealth();
	lastGameState = game->getGameState();
}

#endif // GAME_VISUALIZER_H

