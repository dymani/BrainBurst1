#include "BB/World/Entity.h"
#include "BB/Component/IComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    Entity* Entity::create(luabridge::lua_State* L, luabridge::LuaRef& luaEntity) {
        using namespace luabridge;
        Entity* entity = new Entity();
        LuaRef luaCoord = luaEntity["coord"];
        entity->setCoord({0, 0});
        if(luaCoord.isTable()) {
            entity->setCoord({luaCoord[1].cast<float>(), luaCoord[2].cast<float>()});
        }
        LuaRef luaComponents = luaEntity["components"];
        if(luaComponents.isTable()) {
            LuaRef luaGC = luaComponents["GraphicsComponent"];
            if(luaGC.isTable()) {
                GraphicsComponent* gc = GraphicsComponent::create(*entity, L, luaGC);
                entity->addComponent(std::type_index(typeid(*gc)), gc);
            }
        }
        return entity;
    }

    Entity::Entity() {
    }

    Entity::Entity(const Entity& entity) {
        for(auto& component : entity.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy(*this);
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