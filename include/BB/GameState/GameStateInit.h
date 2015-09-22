#ifndef GAME_STATE_INIT_H
#define GAME_STATE_INIT_H

#include "BB/GameState/IGameState.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Entity.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    class GameStateInit: public IGameState {
    public:
        GameStateInit(Game& game);
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        WindowHandler m_windowHandler;
        GraphicsHandler m_graphicsHandler;
        bool m_isRunning;
    };
}

#endif