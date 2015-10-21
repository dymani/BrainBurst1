#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <map>
#include <SFML/Graphics.hpp>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class IComponent;

    class Entity {
    public:
        Entity();
        Entity(const Entity& entity);
        ~Entity();
        void addComponent(std::type_index type, IComponent* component);
        template <typename T>
        T* getComponent() {
            auto it = m_components.find(std::type_index(typeid(T)));
            if(it != m_components.end()) {
                return dynamic_cast<T*>(it->second);
            }
            LogHandler::log(LogHandler::ERR, std::string(typeid(T).name()) + " not found in Entity",
                typeid(this).name());
            return nullptr;
        }
        template <typename T>
        void removeComponent() {
            auto it = m_components.find(std::type_index(typeid(T)));
            if(it != m_components.end()) {
                delete it->second;
                m_components.erase(it);
            }
        }
        void setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
    private:
        std::map<std::type_index, IComponent*> m_components;
        sf::Vector2f m_coord;
    };
}

#endif