#include "BB/World/EntityTemplate.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/Component/GraphicsComponent.h"
#include "BB/World/Component/MovementComponent.h"
#include "BB/World/Component/PlayerComponent.h"

namespace bb {
    EntityTemplate::EntityTemplate(GameStateGame& game, luabridge::LuaRef& luaE) {
        using namespace luabridge;
        m_name = luaE["name"].cast<std::string>();
        luabridge::LuaRef luaSize = luaE["size"];
        m_size = {luaSize[1].cast<float>(), luaSize[2].cast<float>()};
        luabridge::LuaRef luaComponents = luaE["components"];
        luabridge::LuaRef luaGC = luaComponents["GraphicsComponent"];
        if(!luaGC.isNil()) {
            m_components[std::type_index(typeid(GraphicsComponent))] = game.getWorld()
                .getGraphicsSystem().createComponent(luaE);
        }
        luabridge::LuaRef luaMC = luaComponents["MovementComponent"];
        if(!luaMC.isNil()) {
            m_components[std::type_index(typeid(MovementComponent))] = game.getWorld()
                .getPhysicsSystem().createComponent(luaE);
        }
        luabridge::LuaRef luaPC = luaComponents["PlayerComponent"];
        if(!luaPC.isNil()) {
            m_components[std::type_index(typeid(PlayerComponent))] = game.getWorld()
                .getControlSystem().createComponent(luaE);
        }
    }

    std::string EntityTemplate::getName() {
        return m_name;
    }

    Entity* EntityTemplate::createEntity(GameStateGame& game, rapidjson::Value& jsonE) {
        Entity* entity = new Entity();
        entity->setId(jsonE["id"].GetInt());
        entity->setCoord({float(jsonE["coordX"].GetDouble()), float(jsonE["coordY"].GetDouble())});
        IComponent* c;
        auto* gc = m_components[std::type_index(typeid(GraphicsComponent))];
        if(gc) {
            c = game.getWorld().getGraphicsSystem().createComponent(gc, jsonE);
            entity->addComponent(game, std::type_index(typeid(GraphicsComponent)), c);
        }
        auto* mc = m_components[std::type_index(typeid(MovementComponent))];
        if(mc) {
            c = game.getWorld().getPhysicsSystem().createComponent(mc, jsonE);
            entity->addComponent(game, std::type_index(typeid(MovementComponent)), c);
        }
        auto* pc = m_components[std::type_index(typeid(PlayerComponent))];
        if(pc) {
            c = game.getWorld().getControlSystem().createComponent(pc, jsonE);
            entity->addComponent(game, std::type_index(typeid(PlayerComponent)), c);
        }
        return entity;
    }
}