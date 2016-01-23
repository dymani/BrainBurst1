#ifndef CONTROL_COMPONENT_H
#define CONTROL_COMPONENT_H

#include <memory>
#include <LuaBridge/LuaBridge.h>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class ControlComponent : public IComponent {
        friend class ControlSystem;
    public:
        ControlComponent() : IComponent() {
        }
    private:
        bool m_control;
        bool m_facingLeft, m_movingLeft;
        enum State {
            IDLE, WALKING, JUMPING, CROUCHING
        } m_state;
        std::shared_ptr<luabridge::LuaRef> m_onInput;
    };
}

#endif