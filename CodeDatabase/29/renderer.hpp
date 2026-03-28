#ifndef RENDERER_HPP
#define RENDERER_HPP

class Game;

class TerminalRenderer {
public:
	void Draw(const Game& game);
};

template <typename RenderImpl>
class RenderHub {
public:
	explicit RenderHub(const RenderImpl& impl = RenderImpl()): impl_(impl) {}
	void OnGameUpdated(const Game& game) {
		impl_.Draw(game);
	}
private:
	RenderImpl impl_;
};

#endif
