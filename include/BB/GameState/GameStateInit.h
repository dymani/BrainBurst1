#ifndef GAME_STATE_INIT_H
#define GAME_STATE_INIT_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <SFML/Graphics.hpp>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/LogHandler.h"

namespace bb {
    class GameStateInit: public IGameState {
    public:
        GameStateInit(Game& game);
        ~GameStateInit();
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        WindowHandler* m_windowHandler;
        ResourceHandler* m_resourceHandler;
        luabridge::lua_State* L;
        bool m_isRunning;
        sf::Sprite m_sprite;
        int m_updateCount;
    };
}

#endif