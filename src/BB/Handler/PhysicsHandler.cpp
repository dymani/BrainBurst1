#include "BB/Handler/PhysicsHandler.h"
#include "BB/World/Entity.h"
#include "BB/Component/MovementComponent.h"
#include "BB/Component/CollisionComponent.h"

namespace bb {
    PhysicsHandler::PhysicsHandler(GameStateGame& game) : m_game(game) {
    }

    void PhysicsHandler::update(std::map<int, Entity*>& entities) {
        std::vector<int> movables;
        for(auto& entity : entities) {
            if(entity.second->getComponent<MovementComponent>())
                movables.push_back(entity.second->getId());
        }
        std::vector<int> collidables;
        for(auto& entity : entities) {
            if(entity.second->getComponent<CollisionComponent>())
                collidables.push_back(entity.second->getId());
        }
        for(auto& mEntity : movables) {
            auto* mc = entities[mEntity]->getComponent<MovementComponent>();
            if(!mc->isOnGround()) {
                mc->addVelocity(0, -1.0F);
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
            mc->move();
            if(entities[mEntity]->getCoord().y == 0 && mc->getNewCoord().y < 0) {
                mc->isOnGround(true);
                mc->setNewCoordY(0);
            }
            for(auto& cEntity : collidables) {
                if(mEntity == cEntity) continue;
                auto* cc1 = entities[mEntity]->getComponent<CollisionComponent>();
                auto* cc2 = entities[cEntity]->getComponent<CollisionComponent>();
                cc1->getHitbox();
                cc2->getHitbox();
                if(cc1->getHitbox().intersects(cc2->getHitbox())) {
                    cc1->collide(cEntity);
                    cc2->collide(mEntity);
                }
            }
            mc->update();
        }
    }
}