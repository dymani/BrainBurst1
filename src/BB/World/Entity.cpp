#include "BB/World/Entity.h"
#include "BB/Component/IComponent.h"

namespace bb {
    Entity::Entity() {
    }

    Entity::Entity(const Entity& entity) {
        for(auto& component : entity.m_components) {
            m_components[std::type_index(typeid(*component.second))] = component.second->copy();
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