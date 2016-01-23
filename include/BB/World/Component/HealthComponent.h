#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include <vector>
#include <memory>
#include <LuaBridge/LuaBridge.h>
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
        std::shared_ptr<luabridge::LuaRef> m_onHit;
        std::shared_ptr<luabridge::LuaRef> m_onHealthChange;
        std::shared_ptr<luabridge::LuaRef> m_onDeath;
    };
}

#endif