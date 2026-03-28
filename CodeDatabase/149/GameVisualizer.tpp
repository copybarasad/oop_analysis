#ifndef GAMEVISUALIZER_TPP
#define GAMEVISUALIZER_TPP

template<typename Renderer>
GameVisualizer<Renderer>::GameVisualizer(GameEngine* engine) 
    : gameEngine(engine) {}

template<typename Renderer>
void GameVisualizer<Renderer>::displayGame(int currentLevel) {
    renderer.render(*gameEngine, currentLevel);
}

template<typename Renderer>
void GameVisualizer<Renderer>::onGameStateChanged(int currentLevel) {
    displayGame(currentLevel);
}

#endif