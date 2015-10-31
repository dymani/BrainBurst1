#include "BB/Handler/PhysicsHandler.h"
#include "BB/World/Entity.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    PhysicsHandler::PhysicsHandler(GameStateGame& game) : m_game(game) {
    }

    void PhysicsHandler::update(std::vector<Entity*>& entities) {
        std::vector<MovementComponent*> movables;
        for(auto& entity : entities) {
            if(entity->getComponent<MovementComponent>())
                movables.push_back(entity->getComponent<MovementComponent>());
        }
        for(auto& mc : movables) {
            if(mc->getNewCoord().y > 0) {
                mc->addVelocity(0, -1);
            } else if(mc->getNewCoord().y < 0) {
                mc->setNewCoord({mc->getNewCoord().x, 0});
                mc->setVelocityY(0);
            }
            if(mc->getVelocity().x > 0) {
                mc->addVelocity(-0.5F, 0);
                if(mc->getVelocity().x < 0)
                    mc->setVelocityX(0);
            } else if(mc->getVelocity().x < 0) {
                mc->addVelocity(0.5F, 0);
                if(mc->getVelocity().x > 0)
                    mc->setVelocityX(0);
            }
            mc->update(true);
        }
    }
}