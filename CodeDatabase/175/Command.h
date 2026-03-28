#pragma once
#include <string>
#include <memory>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
    virtual std::string toString() const = 0;
    virtual bool needsParameters() const { return false; }
};

class MoveCommand : public Command {
    int dx_, dy_;
public:
    MoveCommand(int dx = 0, int dy = 0) : dx_(dx), dy_(dy) {}
    void execute(Game& game) override;
    std::string toString() const override {
        return "MoveCommand(" + std::to_string(dx_) + "," + std::to_string(dy_) + ")";
    }
};

class SwitchAttackModeCommand : public Command {
public:
    void execute(Game& game) override;
    std::string toString() const override {
        return "SwitchAttackModeCommand";
    }
};

class UseSpellCommand : public Command {
    int spellIndex_, targetX_, targetY_;
public:
    UseSpellCommand(int index = 0, int x = 0, int y = 0) 
        : spellIndex_(index), targetX_(x), targetY_(y) {}
    void execute(Game& game) override;
    std::string toString() const override {
        return "UseSpellCommand(" + std::to_string(spellIndex_) + ")";
    }
    bool needsParameters() const override { return true; }
};

class EndTurnCommand : public Command {
public:
    void execute(Game& game) override;
    std::string toString() const override {
        return "EndTurnCommand";
    }
};

class SaveCommand : public Command {
    std::string filename_;
public:
    SaveCommand(const std::string& filename = "save.txt") : filename_(filename) {}
    void execute(Game& game) override;
    std::string toString() const override {
        return "SaveCommand(" + filename_ + ")";
    }
};

class LoadCommand : public Command {
    std::string filename_;
public:
    LoadCommand(const std::string& filename = "save.txt") : filename_(filename) {}
    void execute(Game& game) override;
    std::string toString() const override {
        return "LoadCommand(" + filename_ + ")";
    }
};

class ExitCommand : public Command {
public:
    void execute(Game& game) override;
    std::string toString() const override {
        return "ExitCommand";
    }
};