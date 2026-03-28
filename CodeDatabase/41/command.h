#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <string>

class Game;

class Command {
public:
    virtual ~Command() = default;
    virtual void execute(Game& game) = 0;
    virtual std::unique_ptr<Command> clone() const = 0;
    virtual std::string get_name() const = 0;
};

class Move_command : public Command {
private:
    int dx, dy;
public:
    Move_command(int delta_x, int delta_y);
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
    int get_dx() const;
    int get_dy() const;
};

class Attack_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Switch_mode_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Cast_spell_command : public Command {
private:
    int spell_index;
    int target_x, target_y;
public:
    Cast_spell_command(int index, int x = -1, int y = -1);
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
    int get_spell_index() const { return spell_index; }
    int get_target_x() const { return target_x; }
    int get_target_y() const { return target_y; }
};

class Save_command : public Command {
private:
    std::string filename;
public:
    Save_command(const std::string& file = "autosave.txt");
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Load_command : public Command {
private:
    std::string filename;
public:
    Load_command(const std::string& file = "autosave.txt");
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Quit_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Heal_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Next_level_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

class Learn_spell_command : public Command {
public:
    void execute(Game& game) override;
    std::unique_ptr<Command> clone() const override;
    std::string get_name() const override;
};

#endif