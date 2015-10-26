#include "BB/Component/MovementComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    MovementComponent* MovementComponent::create(GameStateGame& game, int entity,
        luabridge::lua_State* L, luabridge::LuaRef& luaMC) {
        MovementComponent* mc = new MovementComponent(game, entity);
        using namespace luabridge;
        LuaRef luaVelocity = luaMC["velocity"];
        mc->m_velocity = {luaVelocity[1].cast<float>(), luaVelocity[2].cast<float>()};
        return mc;
    }

    MovementComponent::MovementComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* MovementComponent::copy(int entity) {
        MovementComponent* mc = new MovementComponent(m_game, entity);
        mc->m_velocity = m_velocity;
        return mc;
    }

    void MovementComponent::update(bool move) {
        if(!m_init) {
            m_newCoord = m_game.getEntity(m_entity)->getCoord();
            m_init = true;
        }
        if(move) {
            m_game.getEntity(m_entity)->setCoord(m_newCoord);
            m_newCoord += sf::Vector2f(m_velocity.x * 0.015625F, m_velocity.y * 0.015625F);
            m_velocity.x = m_velocity.x >= 0.1F ? m_velocity.x - 0.1F : 0;
            m_velocity.x = m_velocity.x <= -0.1F ? m_velocity.x + 0.1F : 0;
            m_velocity.y = m_velocity.y >= 0.1F ? m_velocity.y - 0.1F : 0;
            m_velocity.y = m_velocity.y <= -0.1F ? m_velocity.y + 0.1F : 0;
        }
    }

    void MovementComponent::setVelocity(sf::Vector2f velocity) {
        m_velocity = velocity;
    }

    sf::Vector2f MovementComponent::getNewCoord() {
        return m_newCoord;
    }
    sf::Vector2f MovementComponent::getVelocity() {
        return m_velocity;
    }
}