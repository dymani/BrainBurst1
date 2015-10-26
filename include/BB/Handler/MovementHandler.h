#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H

#include <vector>

namespace bb {
    class Entity;

    class MovementHandler {
    public:
        MovementHandler();
        void update(std::vector<Entity*>& entities);
    private:

    };
}

#endif