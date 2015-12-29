#include "BB/World/EntityTemplate.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    EntityTemplate::EntityTemplate(GameStateGame& game, luabridge::LuaRef& luaE) {
        using namespace luabridge;
        m_name = luaE["name"].cast<std::string>();
        luabridge::LuaRef luaSize = luaE["size"];
        m_size = {luaSize[1].cast<float>(), luaSize[2].cast<float>()};
        luabridge::LuaRef luaComponents = luaE["components"];
        for(auto& system : game.getWorld().getSystems()) {
            system.second->createComponent(luaE, m_components);
        }
    }

    std::string EntityTemplate::getName() {
        return m_name;
    }

    Entity* EntityTemplate::createEntity(GameStateGame& game, rapidjson::Value& jsonE) {
        Entity* entity = new Entity();
        entity->setId(jsonE["id"].GetInt());
        entity->setCoord({float(jsonE["coordX"].GetDouble()), float(jsonE["coordY"].GetDouble())});
        for(auto& system : game.getWorld().getSystems()) {
            system.second->createComponent(jsonE, m_components, entity);
        }
        return entity;
    }
}