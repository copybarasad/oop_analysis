#pragma once

#include <ostream>
#include <utility>

#include "EventBus.hpp"
#include "GameEventListener.hpp"

template <typename RenderPolicy>
class GameVisualizer : public GameEventListener {
public:
	GameVisualizer(EventBus& bus,
	               RenderPolicy renderer,
	               std::ostream& output)
		: bus_(bus),
		  renderer_(std::move(renderer)),
		  output_(output) {
		bus_.subscribe(*this);
	}

	~GameVisualizer() override {
		bus_.unsubscribe(*this);
	}

	void onEvent(const GameEvent& event) override {
		if (event.type() != GameEvent::Type::StateChanged) {
			return;
		}
		if (!event.field() || !event.world()) {
			return;
		}
		renderer_.draw(*event.field(), *event.world(), output_);
	}

private:
	EventBus& bus_;
	RenderPolicy renderer_;
	std::ostream& output_;
};
