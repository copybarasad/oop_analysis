#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <iostream>
#include "field.h"
#include "player.h"

class RendererInterface {
public:
    virtual ~RendererInterface() = default;
    virtual void renderField(const Field* field) = 0;
    virtual void renderPlayerInfo(const Player* player) = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void clearScreen() = 0;
    virtual void renderGameOver(int playerScore, int level) = 0;
};

class ConsoleRenderer : public RendererInterface {
private:
    static constexpr const char* EMPTY_CELL = "  .";
    static constexpr const char* PLAYER_CELL = "  P";
    static constexpr const char* ENEMY_CELL = "  E";
    static constexpr const char* BUILDING_CELL = "  B";
    static constexpr const char* ALLY_CELL = "  A";
    static constexpr const char* TRAP_CELL = "  T";

    void renderBorder(int cols) const {
        for (int i = 0; i < cols * 3 + 2; ++i) {
            std::cout << "-";
        }
        std::cout << "\n";
    }

public:
    void renderField(const Field* field) override {
        if (!field) return;

        int rows = field->rows();
        int cols = field->cols();

        renderBorder(cols);

        for (int r = 0; r < rows; ++r) {
            std::cout << "|";
            for (int c = 0; c < cols; ++c) {
                int occupantId = field->getOccupantIdAt(r, c);
                if (occupantId == -1) {
                    std::cout << EMPTY_CELL;
                } else {
                    const Entity* entity = field->getEntity(occupantId);
                    if (entity) {
                        std::cout << getCellRepresentationByEntity(entity);
                    } else {
                        std::cout << EMPTY_CELL;
                    }
                }
            }
            std::cout << "|\n";
        }

        renderBorder(cols);
    }

    void renderPlayerInfo(const Player* player) override {
        if (!player) return;

        std::cout << "\n--- PLAYER INFO ---\n";
        std::cout << "Health: " << player->getHealth() << "\n";
        std::cout << "Melee Damage: " << player->getMeleeDamage() << "\n";
        std::cout << "Ranged Damage: " << player->getRangedDamage() << "\n";
        std::cout << "Score: " << player->getScore() << "\n";
    }

    void renderMessage(const std::string& message) override {
        std::cout << message << "\n";
    }

    void clearScreen() override {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void renderGameOver(int playerScore, int level) override {
        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "GAME OVER!\n";
        std::cout << "====================================\n";
        std::cout << "Final Score: " << playerScore << "\n";
        std::cout << "Reached Level: " << level << "\n";
        std::cout << "====================================\n";
    }

private:
    const char* getCellRepresentationByEntity(const Entity* entity) const {
        if (!entity) return EMPTY_CELL;
        
        EntityType type = entity->getType();
        switch (type) {
            case EntityType::Player:
                return PLAYER_CELL;
            case EntityType::Enemy:
                return ENEMY_CELL;
            case EntityType::Building:
                return BUILDING_CELL;
            case EntityType::Ally:
                return ALLY_CELL;
            case EntityType::Trap:
                return TRAP_CELL;
            default:
                return EMPTY_CELL;
        }
    }
};

#endif
