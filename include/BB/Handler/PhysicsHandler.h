#ifndef PHYSICS_HANDLER_H
#define PHYSICS_HANDLER_H

#include <vector>

namespace bb {
    class GameStateGame;
    class Entity;

    class PhysicsHandler {
    public:
        PhysicsHandler(GameStateGame& game);
        void update(std::vector<Entity*>& entities);
    private:
        GameStateGame& m_game;
    };
}

#endif