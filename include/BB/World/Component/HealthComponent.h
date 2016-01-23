#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <vector>
#include <memory>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class HealthComponent : public IComponent {
        friend class HealthSystem;
    public:
        HealthComponent() : IComponent() {
        }
    private:
        int m_maxHealth;
        int m_health;
        int m_damage;
        std::map<int, std::string> m_frames;
        std::shared_ptr<luabridge::LuaRef> m_deathFunc;
    };
}

#endif