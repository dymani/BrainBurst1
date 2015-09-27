#include "BB/Entity.h"
#include "BB/Component/IComponent.h"

namespace bb {
    Entity::Entity() {
    }

    Entity::Entity(const Entity& rhs) {
        for(auto& component : rhs.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy();
        }
        m_coord = rhs.m_coord;
    }

    Entity::~Entity() {
    }

    Entity* Entity::addComponent(std::type_index type, IComponent* component) {
        m_components[type] = component;
        return this;
    }

    Entity* Entity::setCoord(sf::Vector2f coord) {
        m_coord = coord;
        return this;
    }

    sf::Vector2f Entity::getCoord() {
        return m_coord;
    }
}