#ifndef GAME_STATE_GAME_H
#define GAME_STATE_GAME_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/World/Entity.h"
#include "BB/GameState/IGameState.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/MovementHandler.h"
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
        Entity* getEntity(int id);
        std::vector<Entity*>& getEntities();
        ResourceHandler* getResourceHandler();
        GraphicsHandler* getGraphicsHandler();
        WindowHandler* getWindowHandler();
        MovementHandler* getMovementHandler();
    private:
        ResourceHandler* m_resourceHandler;
        GraphicsHandler* m_graphicsHandler;
        WindowHandler* m_windowHandler;
        MovementHandler* m_movementHandler;
        luabridge::lua_State* L;
        enum State {
            RUNNING, TITLE, QUIT
        } m_state;
        Field* m_field;
        std::vector<Entity*> m_entities;
    };
}

#endif