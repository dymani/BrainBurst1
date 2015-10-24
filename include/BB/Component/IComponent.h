#ifndef I_COMPONENT_H
#define I_COMPONENT_H

namespace bb {
    class Entity;

    class IComponent {
    public:
        virtual ~IComponent() {
        };
        virtual IComponent* copy(Entity& entity) = 0;
    protected:

    };
}

#endif