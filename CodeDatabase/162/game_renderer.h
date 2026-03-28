#ifndef GAME_RENDERER_H_
#define GAME_RENDERER_H_

#include "field.h"
#include "player.h"
#include "hand.h"
#include "renderer.h"
#include <memory>
#include <functional>
#include <string>

template<typename T>
class RendererInterface {
public:
    virtual ~RendererInterface() = default;
    virtual void RenderField(const Field& field) const = 0;
    virtual void RenderPlayerInfo(const Player& player) const = 0;
    virtual void RenderHand(const Hand& hand) const = 0;
    virtual void RenderGameState(const Field& field) const = 0;
    virtual void RenderMessage(const std::string& message) const = 0;
    virtual void RenderHelp() const = 0;
};

class ConsoleRenderer : public RendererInterface<Renderer> {
public:
    void RenderField(const Field& field) const override {
        Renderer renderer;
        renderer.RenderField(field);
    }
    
    void RenderPlayerInfo(const Player& player) const override {
        Renderer renderer;
        renderer.RenderPlayerInfo(player);
    }
    
    void RenderHand(const Hand& hand) const override {
        Renderer renderer;
        renderer.RenderHand(hand);
    }
    
    void RenderGameState(const Field& field) const override {
        Renderer renderer;
        renderer.RenderGameState(field);
    }
    
    void RenderMessage(const std::string& message) const override {
        Renderer renderer;
        renderer.RenderMessage(message);
    }
    
    void RenderHelp() const override {
        Renderer renderer;
        renderer.RenderHelp();
    }
};

template<typename RendererType>
class GameRenderer {
public:
    GameRenderer() : renderer_(std::make_shared<RendererType>()) {}
    ~GameRenderer() = default;
    
    void RenderFullGameState(const Field& field) {
        renderer_->RenderGameState(field);
    }
    
    void RenderFieldOnly(const Field& field) {
        renderer_->RenderField(field);
    }
    
    void RenderPlayerStatus(const Player& player) {
        renderer_->RenderPlayerInfo(player);
    }
    
    void RenderPlayerHand(const Hand& hand) {
        renderer_->RenderHand(hand);
    }
    
    void RenderMessage(const std::string& message) {
        renderer_->RenderMessage(message);
    }
    
    void RenderHelpScreen() {
        renderer_->RenderHelp();
    }
    
    void RenderChanges(const Field& field, const std::string& changeType = "full") {
        if (changeType == "full") {
            RenderFullGameState(field);
        } else if (changeType == "field") {
            RenderFieldOnly(field);
        } else if (changeType == "player") {
            if (field.GetPlayer()) {
                RenderPlayerStatus(*field.GetPlayer());
            }
        } else if (changeType == "hand") {
            if (field.GetPlayer()) {
                RenderPlayerHand(field.GetPlayer()->GetHand());
            }
        }
    }
    
    RendererType* GetRenderer() const {
        return renderer_.get();
    }
    
private:
    std::shared_ptr<RendererType> renderer_;
};

#endif // GAME_RENDERER_H_