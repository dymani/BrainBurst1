#ifndef DAMAGE_COMPONENT_H
#define DAMAGE_COMPONENT_H

#include "BB/World/Component/IComponent.h"

namespace bb {
    class DamageComponent : public IComponent {
        friend class HealthSystem;
    public:
        DamageComponent(int damage) :IComponent() {
            m_damage = damage;
        }
    private:
        int m_damage;
    };
}

#endif