#ifndef INPUT_READER_H
#define INPUT_READER_H

#include "Command.h"
#include "KeyBindings.h"
#include <SFML/Window.hpp>

class IInputReader {
public:
    virtual ~IInputReader() = default;
    virtual Command ReadInput() = 0;
    virtual bool HasInput() const = 0;
    virtual void SetSpellTarget(int spell_index, int target_x, int target_y) = 0;
};

class SfmlInputReader : public IInputReader {
public:
    SfmlInputReader(sf::Window& window, KeyBindings& bindings);

    Command ReadInput() override;
    bool HasInput() const override;
    void SetSpellTarget(int spell_index, int target_x, int target_y) override;

    void ProcessEvent(const sf::Event& event);
    void ClearPendingCommand();

private:
    sf::Window& window_;
    KeyBindings& bindings_;
    Command pending_command_;
    bool has_pending_command_;
};

#endif // INPUT_READER_H
