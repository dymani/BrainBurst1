#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include "BB/Component/UpdateComponent.h"
#include <vector>

namespace bb {
    class IGameState;
    class Entity;

    class UpdateHandler {
    public:
        UpdateHandler(std::vector<Entity*>& entities, IGameState* gameState);
        void update();
        IGameState* getGameState();
    private:
        std::vector<Entity*>& m_entities;
        IGameState* m_gameState;
    };
}

#endif