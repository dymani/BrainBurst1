#ifndef MOVEMENT_HANDLER_H
#define MOVEMENT_HANDLER_H

#include <vector>

namespace bb {
    class GameStateGame;
    class Entity;

    class MovementHandler {
    public:
        MovementHandler(GameStateGame& game);
        void update(std::vector<Entity*>& entities);
    private:
        GameStateGame& m_game;
    };
}

#endif