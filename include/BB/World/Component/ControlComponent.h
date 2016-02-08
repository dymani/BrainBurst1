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
        bool m_control, m_script;
        std::string m_scriptName;
    };
}

#endif