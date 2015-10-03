#ifndef GAME_STATE_SPLASH_H
#define GAME_STATE_SPLASH_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/GameState/IGameState.h"
#include "BB/Handler/AudioHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/GuiHandler.h"
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/ScriptHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"

namespace bb {
    class Game;

    class GameStateSplash: public IGameState{
    public:
        GameStateSplash(Game& game, ResourceHandler* resourceHandler, luabridge::lua_State* L);
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        AudioHandler* m_audioHandler;
        GraphicsHandler* m_graphicsHandler;
        GuiHandler* m_guiHandler;
        ResourceHandler* m_resourceHandler;
        ScriptHandler* m_scriptHandler;
        WindowHandler* m_windowHandler;
        luabridge::lua_State* L;
        int m_ticks;
        bool m_isRunning;
    };
}

#endif