#include "BB/Component/PlayerComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    PlayerComponent* PlayerComponent::create(GameStateGame& game, luabridge::lua_State* L,
        luabridge::LuaRef& luaPC) {
        PlayerComponent* pc = new PlayerComponent(game, 0);
        return pc;
    }

    PlayerComponent::PlayerComponent(GameStateGame& game, int entity) : IComponent(game, 0) {
    }

    IComponent * PlayerComponent::copy(int entity) {
        PlayerComponent* pc = new PlayerComponent(m_game, 0);
        return pc;
    }

    void PlayerComponent::handleInput() {
        bool keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        if(!keyA ^ keyD)
            return;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_game.getEntity(0)->getComponent<MovementComponent>()->setVelocity({-2.0F, 0.0F});
        } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_game.getEntity(0)->getComponent<MovementComponent>()->setVelocity({2.0F, 0.0F});
        }
    }
}