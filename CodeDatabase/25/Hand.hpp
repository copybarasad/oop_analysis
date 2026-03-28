#pragma once

#include <memory>
#include <vector>

#include "spell/ISpell.hpp"

// ==========================================================
// SpellHand (Hand.hpp) — "рука" заклинаний / инвентарь спеллов
//
// Идея:
// - хранит заклинания в виде std::unique_ptr<ISpell>
// - у руки фиксированная вместимость capacity
// - копирование руки возможно (copy ctor / operator=),
//   но поскольку unique_ptr копировать нельзя,
//   копирование реализовано через полиморфный метод clone() у ISpell.
// ==========================================================

// ----------------------------
// 1) Конфигурация руки
// ----------------------------
// Пока здесь только capacity (сколько максимум заклинаний можно хранить).
struct HandConfig {
    std::size_t capacity{1};
};

class SpellHand {
    // ----------------------------
    // 2) Хранилище заклинаний
    // ----------------------------
    // slots_ — динамический массив слотов.
    // Каждый слот — unique_ptr<ISpell>, то есть владеет объектом заклинания.
    std::vector<std::unique_ptr<ISpell>> slots_;

    // настройки (вместимость)
    HandConfig cfg_;

    // ----------------------------
    // 3) Внутренний метод копирования
    // ----------------------------
    // Копирует заклинания из другой руки.
    // Важно: unique_ptr копировать нельзя, поэтому используем spell->clone().
    void copy_from(const SpellHand& other) {
        slots_.clear();
        slots_.reserve(other.slots_.size());

        for (const auto& spell : other.slots_) {
            if (spell) {
                // clone() создаёт новый объект того же типа (SpellDirect/SpellArea и т.д.)
                slots_.push_back(spell->clone());
            }
        }
    }

public:
    // ----------------------------
    // 4) Конструктор
    // ----------------------------
    // По умолчанию создаёт руку с capacity=1 (как в HandConfig).
    explicit SpellHand(HandConfig cfg = {}) : cfg_(cfg) {}

    // ----------------------------
    // 5) Копирующий конструктор
    // ----------------------------
    // Создаёт полную копию руки: cfg + копии всех заклинаний.
    SpellHand(const SpellHand& other) : cfg_(other.cfg_) {
        copy_from(other);
    }

    // ----------------------------
    // 6) Оператор копирующего присваивания
    // ----------------------------
    // Аналогично: защищаемся от self-assignment и копируем содержимое.
    SpellHand& operator=(const SpellHand& other) {
        if (this == &other) return *this;
        cfg_ = other.cfg_;
        copy_from(other);
        return *this;
    }

    // ----------------------------
    // 7) Перемещающие операции
    // ----------------------------
    // Перемещение можно делать "по умолчанию" (unique_ptr перемещается).
    SpellHand(SpellHand&&) noexcept = default;
    SpellHand& operator=(SpellHand&&) noexcept = default;

    // ----------------------------
    // 8) Добавить заклинание в руку
    // ----------------------------
    // Принимаем unique_ptr (владение передаётся руке).
    // Если рука заполнена — вернуть false.
    bool add(std::unique_ptr<ISpell> s) {
        if (full()) {
            return false;
        }
        slots_.push_back(std::move(s));
        return true;
    }

    // ----------------------------
    // 9) Получить заклинание по индексу
    // ----------------------------
    // Возвращаем "сырой" указатель (рука всё ещё владеет объектом).
    // Если индекс вне диапазона — nullptr.
    ISpell* get(std::size_t i) {
        return (i < slots_.size()) ? slots_[i].get() : nullptr;
    }

    const ISpell* get(std::size_t i) const {
        return (i < slots_.size()) ? slots_[i].get() : nullptr;
    }

    // ----------------------------
    // 10) Методы для информации о руке
    // ----------------------------
    std::size_t capacity() const { return cfg_.capacity; }      // максимальное число спеллов
    std::size_t size() const { return slots_.size(); }          // текущее число спеллов
    bool full() const { return slots_.size() >= cfg_.capacity; } // заполнена ли рука

    // ----------------------------
    // 11) Доступ к слотам (только чтение)
    // ----------------------------
    // Используется для HUD/рендера и для сохранения.
    const std::vector<std::unique_ptr<ISpell>>& slots() const {
        return slots_;
    }
};
