#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "BB/World/Component/IComponent.h"

namespace bb {
    class PlayerComponent : public IComponent {
        friend class ControlSystem;
    public:
        PlayerComponent() : IComponent() {
        }
    private:
        bool m_facingLeft, m_movingLeft;
        enum State {
            IDLE, WALKING, JUMPING, CROUCHING
        } m_state;

    };
}

#endif