#ifndef I_COMPONENT_H
#define I_COMPONENT_H

namespace bb {
    class IComponent {
    public:
        virtual ~IComponent() {
        };
        virtual IComponent* copy() = 0;
    protected:

    };
}

#endif