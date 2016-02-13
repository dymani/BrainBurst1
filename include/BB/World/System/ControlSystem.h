#ifndef CONTROL_SYSTEM_H
#define CONTROL_SYSTEM_H

#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/World/System/ISystem.h"
#include "BB/World/Component/ControlComponent.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
    class GameStateGame;

    class ControlSystem : public ISystem {
    public:
        static void registerState(luabridge::lua_State* L);
        ControlSystem(GameStateGame& game);
        void createList(std::map<std::type_index, std::unique_ptr<CList>>& lists);
        void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list);
        void createComponent(rapidjson::Value& jsonE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void createComponent(std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void handleInput();
        void handleInput(sf::Event& windowEvent);
        int getPlayerId();
    private:
        GameStateGame& m_game;
        std::map<std::string, std::shared_ptr<luabridge::LuaRef>> m_scripts;
        int m_player;
    };

    class LuaInput {
    public:
        bool keyW, keyA, keyS, keyD, keySpace, keyShift, keyCtrl, mouseLeft;
        float mouseX, mouseY;
    };
}

#endif