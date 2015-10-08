#ifndef GAME_STATE_TITLE_H
#define GAME_STATE_TITLE_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/GameState/IGameState.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"

namespace bb {
    class Game;

    class GameStateTitle : public IGameState {
    public:
        GameStateTitle(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
            luabridge::lua_State* L);
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        ResourceHandler* m_resourceHandler;
        WindowHandler* m_windowHandler;
        luabridge::lua_State* L;
        enum State {
            RUNNING, QUIT, GAME
        } m_state;
    };
}

#endif