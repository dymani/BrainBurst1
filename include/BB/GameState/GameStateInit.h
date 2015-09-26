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
#include "BB/Entity.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/ScriptHandler.h"

namespace bb {
    class GameStateInit: public IGameState {
    public:
        GameStateInit(Game& game);
        ~GameStateInit();
        bool update();
        void draw(const double dt);
        void handleInput();
    private:
        WindowHandler m_windowHandler;
        GraphicsHandler m_graphicsHandler;
        ResourceHandler m_resourceHandler;
        ScriptHandler m_scriptHandler;
        bool m_isRunning;
        luabridge::lua_State* L;
        std::shared_ptr<luabridge::LuaRef> m_initFunc;
        std::vector<Entity*> m_entities;
    };
}

#endif