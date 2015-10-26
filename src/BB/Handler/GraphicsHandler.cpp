#include "BB/World/Entity.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(GameStateGame& game) : m_game(game) {
    }

    void GraphicsHandler::draw(int entity) {
        m_entities.push_back(entity);
    }

    void GraphicsHandler::display(const double dt) {
        sf::RenderWindow& window = m_game.getWindowHandler()->getWindow();
        std::vector<Entity*> entities;
        for(auto& entity : m_entities) {
            if(m_game.getEntity(entity))
                if(m_game.getEntity(entity)->getComponent<GraphicsComponent>())
                    entities.push_back(m_game.getEntity(entity));
        }
        m_entities.clear();
        std::sort(entities.begin(), entities.end(), compareEntities);
        for(auto& entity : entities) {
            if(entity->getComponent<MovementComponent>()) {
                entity->getComponent<GraphicsComponent>()->draw(window, {
                    float(entity->getComponent<MovementComponent>()->getVelocity().x * dt),
                    float(entity->getComponent<MovementComponent>()->getVelocity().y * dt)});
            }
            entity->getComponent<GraphicsComponent>()->draw(window);
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        return (a->getComponent<GraphicsComponent>()->getZ() < b->getComponent<GraphicsComponent>()->getZ());
    }
}