#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <map>
#include <SFML/Graphics.hpp>

namespace bb {
    class IComponent;

    class Entity {
    public:
        Entity();
        Entity(const Entity& rhs);
        ~Entity();
        Entity* addComponent(std::type_index type, IComponent* component);
        template <typename T>
        T* get() {
            auto it = m_components.find(std::type_index(typeid(T)));
            if(it != m_components.end()) {
                return dynamic_cast<T*>(it->second);
            }
            return nullptr;
        }
        Entity* setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
    private:
        std::map<std::type_index, IComponent*> m_components;
        sf::Vector2f m_coord;

    };
}

#endif