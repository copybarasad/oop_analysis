#ifndef I_INPUT_STRATEGY_H
#define I_INPUT_STRATEGY_H

class IInputStrategy {
public:
    virtual ~IInputStrategy() = default;
    virtual bool handle() = 0;  // Возвращает false, если игра должна быть перезапущена (например, после загрузки)
};

#endif
