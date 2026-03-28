#ifndef ISERIALIZER_H
#define ISERIALIZER_H

#include <nlohmann/json.hpp>

/**
 * Базовый интерфейс для сериализации объектов в JSON.
 * Каждый сериализатор работает с конкретным типом объекта.
 * 
 * Принцип: Single Responsibility - сериализатор отвечает только за преобразование
 * объекта в JSON и обратно, не нарушая инкапсуляцию модели.
 */
template <typename T>
class ISerializer {
public:
    virtual ~ISerializer() = default;
    
    /**
     * Сериализует объект в JSON.
     * @param obj объект для сериализации (const reference для безопасности)
     * @return JSON представление объекта
     */
    virtual nlohmann::json serialize(const T& obj) const = 0;
    
    /**
     * Десериализует объект из JSON.
     * Объект должен быть уже создан, метод заполняет его данными из JSON.
     * @param obj объект для заполнения данными
     * @param json JSON данные для десериализации
     * @throws std::runtime_error при некорректных данных
     */
    virtual void deserialize(T& obj, const nlohmann::json& json) const = 0;
};

#endif

