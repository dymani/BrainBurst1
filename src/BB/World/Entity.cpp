#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/IComponent.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/MovementComponent.h"
#include "BB/Component/PlayerComponent.h"
#include "BB/Component/CollisionComponent.h"
#include "BB/Component/BreakableComponent.h"

namespace bb {
    Entity* Entity::create(GameStateGame& game, luabridge::lua_State* L, luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        Entity* entity = new Entity();
        LuaRef luaComponents = luaEntity["components"];
        LuaRef luaGC = luaComponents["GraphicsComponent"];
        if(luaGC.isTable()) {
            GraphicsComponent* gc = GraphicsComponent::create(game, L, luaGC);
            entity->addComponent(std::type_index(typeid(*gc)), gc);
        }
        LuaRef luaMC = luaComponents["MovementComponent"];
        if(luaMC.isTable()) {
            MovementComponent* mc = MovementComponent::create(game, L, luaMC);
            entity->addComponent(std::type_index(typeid(*mc)), mc);
        }
        LuaRef luaPC = luaComponents["PlayerComponent"];
        if(luaPC.isTable()) {
            PlayerComponent* pc = PlayerComponent::create(game, L, luaPC);
            entity->addComponent(std::type_index(typeid(*pc)), pc);
        }
        LuaRef luaCC = luaComponents["CollisionComponent"];
        if(luaCC.isTable()) {
            CollisionComponent* cc = CollisionComponent::create(game, L, luaCC);
            entity->addComponent(std::type_index(typeid(*cc)), cc);
        }
        LuaRef luaBC = luaComponents["BreakableComponent"];
        if(luaBC.isTable()) {
            BreakableComponent* bc = BreakableComponent::create(game, L, luaBC);
            entity->addComponent(std::type_index(typeid(*bc)), bc);
        }
        return entity;
    }

    Entity::Entity() : m_id(-1), m_isEntity(false) {
    }

    Entity::Entity(Entity& entity, int id) : m_id(id), m_isEntity(true) {
        for(auto& component : entity.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy(id);
        }
        m_coord = entity.m_coord;
    }

    Entity::Entity(Entity& entity, rapidjson::Value& value) : m_isEntity(true) {
        for(auto& component : entity.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy(value);
        }
        if(value.HasMember("id"))
            m_id = value["id"].GetInt();
        else
            m_id = -1;
        m_coord = {float(value["xCoord"].GetDouble()), float(value["yCoord"].GetDouble())};
    }

    Entity::~Entity() {
    }

    int Entity::getId() {
        return m_id;
    }

    void Entity::addComponent(std::type_index type, IComponent* component) {
        m_components[type] = component;
    }

    void Entity::setCoord(sf::Vector2f coord) {
        m_coord = coord;
    }

    sf::Vector2f Entity::getCoord() {
        return m_coord;
    }
    bool Entity::getUpdate() {
        for(auto& component : m_components) {
            if(component.second->getUpdate()) return true;
        }
        return false;
    }
}