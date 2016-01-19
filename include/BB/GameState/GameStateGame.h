#ifndef GAME_STATE_GAME_H
#define GAME_STATE_GAME_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <memory>
#include "BB/GameState/IGameState.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Handler/AudioHandler.h"
#include "BB/Handler/LogHandler.h"
#include "BB/World/World.h"

namespace bb {
    class Game;

    class GameStateGame: public IGameState {
    public:
        GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler);
        void handleInput();
        bool update();
        void draw(const double dt);
        ResourceHandler* getResourceHandler();
        WindowHandler* getWindowHandler();
        luabridge::lua_State* getLuaState();
        World& getWorld();
    private:
        std::unique_ptr<ResourceHandler> m_resourceHandler;
        std::unique_ptr<WindowHandler> m_windowHandler;
        std::unique_ptr<AudioHandler> m_audioHandler;
        luabridge::lua_State* L;
        enum State {
            RUNNING, TITLE, QUIT
        } m_state;
        World m_world;
    };
}

#endif