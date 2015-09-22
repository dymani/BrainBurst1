#ifndef ENTITY_H
#define ENTITY_H

#include <typeindex>
#include <map>

namespace bb {
    class IComponent;

    class Entity {
    public:
        Entity();
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
        Entity* setZ(float z);
        float getZ();
    private:
        std::map<std::type_index, IComponent*> m_components;
        float m_z;
    };
}

#endif