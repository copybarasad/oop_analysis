#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "game_renderer.h"
#include "rendering_manager.h"
#include "event_listener.h"
#include "event_bus.h"
#include <memory>

class Game;

// Шаблонный класс визуализации игры, наследует RenderingManager
// RendererType должен предоставлять интерфейс GameRenderer
// Реагирует на события и вызывает отрисовку
template<typename RendererType>
class GameVisualizer : public RenderingManager, public EventListener {
private:
    Game& game;
    std::unique_ptr<RendererType> renderer;

public:
    explicit GameVisualizer(Game& g)
        : game(g), renderer(std::make_unique<RendererType>()) {
        // Подписываемся на события
        EventBus::getInstance().subscribe(std::shared_ptr<EventListener>(this, [](void*) {}));
    }

    virtual ~GameVisualizer() override {
        // Отписываемся от событий
        EventBus::getInstance().clearListeners();
    }

    // Обработчик событий - реагирует на изменения в игре
    void onEvent(const std::shared_ptr<GameEvent>& event) override {
        // При возникновении события - перерисовываем необходимую часть
        update();
    }

    // Отрисовать поле
    void renderField() override {
        renderer->renderField(game);
    }

    // Отрисовать статус игрока
    void renderPlayerStatus() override {
        renderer->renderPlayerStatus(game);
    }

    // Отрисовать список заклинаний
    void renderSpellList() override {
        renderer->renderSpellList(game);
    }

    // Отрисовать сообщение
    void renderMessage(const std::string& message) override {
        renderer->renderMessage(message);
    }

    // Отрисовать элементы управления
    void renderControls() override {
        renderer->renderControls();
    }

    // Обновить отображение (статус и заклинания)
    void update() override {
        renderPlayerStatus();
        renderSpellList();
    }

    // Получить доступ к игре и рендереру
    Game& getGame() { return game; }
    const Game& getGame() const { return game; }

    RendererType& getRenderer() { return *renderer; }
};

#endif