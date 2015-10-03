#ifndef GAME_STATE_INIT_H
#define GAME_STATE_INIT_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/ScriptHandler.h"
#include "BB/Handler/LogHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Entity.h"

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
        GraphicsHandler* m_graphicsHandler;
        ResourceHandler* m_resourceHandler;
        ScriptHandler* m_scriptHandler;
        luabridge::lua_State* L;
        bool m_isRunning;
    };
}

#endif