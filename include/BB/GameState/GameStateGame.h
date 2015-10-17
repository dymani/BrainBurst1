#ifndef GAME_STATE_GAME_H
#define GAME_STATE_GAME_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/GameState/IGameState.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/LogHandler.h"
#include "BB/World/Field.h"

namespace bb {
    class Game;

    class GameStateGame: public IGameState {
    public:
        GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler,
            luabridge::lua_State* L);
        void handleInput();
        bool update();
        void draw(const double dt);
    private:
        ResourceHandler* m_resourceHandler;
        WindowHandler* m_windowHandler;
        luabridge::lua_State* L;
        enum State {
            RUNNING, TITLE, QUIT
        } m_state;
        Field* m_field;
    };
}

#endif