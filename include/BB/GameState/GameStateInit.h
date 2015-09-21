#ifndef GAME_STATE_INIT_H
#define GAME_STATE_INIT_H

#include "BB/GameState/IGameState.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
    class GameStateInit: public IGameState {
    public:
        GameStateInit(Game& game);
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        WindowHandler m_windowHandler;
        bool m_isRunning;
    };
}

#endif