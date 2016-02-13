#ifndef ENTITY_TEMPLATE_H
#define ENTITY_TEMPLATE_H

#include <map>
#include <typeindex>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include <memory>
#include "BB/World/Component/IComponent.h"
#include "BB/World/Entity.h"

namespace bb {
    class GameStateGame;

    class EntityTemplate {
    public:
        EntityTemplate(GameStateGame& game, luabridge::LuaRef& luaE);
        std::string getName();
        Entity* createEntity(GameStateGame& game, rapidjson::Value& jsonE);
        Entity* createEntity(GameStateGame& game, int id, sf::Vector2f coord);
    private:
        std::string m_name;
        sf::Vector2f m_size;
        std::map<std::type_index, std::unique_ptr<IComponent>> m_components;
    };
}

#endif