#pragma once
#include <type_traits>
#include <vector>
#include "SaveManager.hpp"
class Field;
class Player;

// Шаблонный класс для визуализации игры
// Параметр шаблона: класс рендерера
template<typename RendererType>
class GameVisualizer {
    // Статические проверки наличия всех необходимых методов отрисовки
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().render(std::declval<const Field&>()))>,
                 "RendererType должен иметь метод render(const Field&)");
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().renderStats(std::declval<const Player&>()))>,
                 "RendererType должен иметь метод renderStats(const Player&)");
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().renderMainMenu())>,
                 "RendererType должен иметь метод renderMainMenu()");
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().renderPauseMenu())>,
                 "RendererType должен иметь метод renderPauseMenu()");
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().renderLoadMenu(std::declval<const std::vector<SaveInfo>&>()))>,
                 "RendererType должен иметь метод renderLoadMenu(const std::vector<SaveInfo>&)");
    static_assert(std::is_void_v<decltype(std::declval<RendererType>().renderGameOver(std::declval<bool>(), std::declval<int>()))>,
                 "RendererType должен иметь метод renderGameOver(bool, int)");
    
    RendererType renderer;
public:
    GameVisualizer() = default;
    // Отрисовка игрового состояния (поля и статистики)
    void renderGameState(const Field& field, const Player& player) {
        renderer.render(field);
        renderer.renderStats(player);
    }
    // Отображение главного меню
    void renderMainMenu() {
        renderer.renderMainMenu();
    }
    // Отображение меню паузы
    void renderPauseMenu() {
        renderer.renderPauseMenu();
    }
    // Отображение меню загрузки
    void renderLoadMenu(const std::vector<SaveInfo>& saves) {
        renderer.renderLoadMenu(saves);
    }
    // Отображение экрана завершения игры
    void renderGameOver(bool victory, int score) {
        renderer.renderGameOver(victory, score);
    }
};
