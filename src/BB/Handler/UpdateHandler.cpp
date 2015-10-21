#include "BB/Handler/UpdateHandler.h"
#include "BB/World/Entity.h"

namespace bb {
    UpdateHandler::UpdateHandler(std::vector<Entity*>& entities, IGameState* gameState):m_entities(entities),
        m_gameState(gameState) {
    }

    void UpdateHandler::update() {
        for(unsigned int i = 0; i < m_entities.size(); ) {
            /*auto uc = m_entities[i]->get<UpdateComponent>();
            if(!uc) {
                i++;
                continue;
            }
            if(uc->update(m_entities[i])) {
                delete m_entities[i];
                m_entities.erase(m_entities.begin() + i);
                continue;
            }*/
            i++;
        }
    }

    IGameState* UpdateHandler::getGameState() {
        return m_gameState;
    }
}