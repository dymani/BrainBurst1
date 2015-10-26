#include "BB/Handler/MovementHandler.h"
#include "BB/World/Entity.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    MovementHandler::MovementHandler(GameStateGame& game) : m_game(game) {
    }

    void MovementHandler::update(std::vector<Entity*>& entities) {
        std::vector<MovementComponent*> movables;
        for(auto& entity : entities) {
            if(entity->getComponent<MovementComponent>())
                movables.push_back(entity->getComponent<MovementComponent>());
        }
        for(auto& mc : movables) {
            mc->update(true);
        }
    }
}