#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/IComponent.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/MovementComponent.h"
#include "BB/Component/PlayerComponent.h"

namespace bb {
    Entity* Entity::create(GameStateGame& game, int id, luabridge::lua_State* L,
        luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        Entity* entity = new Entity(id);
        LuaRef luaCoord = luaEntity["coord"];
        entity->setCoord({0, 0});
        if(luaCoord.isTable()) {
            entity->setCoord({luaCoord[1].cast<float>(), luaCoord[2].cast<float>()});
        }
        LuaRef luaComponents = luaEntity["components"];
        if(luaComponents.isTable()) {
            LuaRef luaGC = luaComponents["GraphicsComponent"];
            if(luaGC.isTable()) {
                GraphicsComponent* gc = GraphicsComponent::create(game, id, L, luaGC);
                entity->addComponent(std::type_index(typeid(*gc)), gc);
            }
            LuaRef luaMC = luaComponents["MovementComponent"];
            if(luaMC.isTable()) {
                MovementComponent* mc = MovementComponent::create(game, id, L, luaMC);
                entity->addComponent(std::type_index(typeid(*mc)), mc);
            }
            LuaRef luaPC = luaComponents["PlayerComponent"];
            if(luaPC.isTable()) {
                PlayerComponent* pc = PlayerComponent::create(game, L, luaPC);
                entity->addComponent(std::type_index(typeid(*pc)), pc);
            }
        }
        return entity;
    }

    Entity::Entity(int id) : m_id(id) {
    }

    Entity::Entity(Entity& entity, int id) : m_id(id) {
        for(auto& component : entity.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy(id);
        }
        m_coord = entity.m_coord;
    }

    Entity::~Entity() {
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
}