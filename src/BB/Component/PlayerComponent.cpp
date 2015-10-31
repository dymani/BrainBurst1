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
        m_state = IDLE;
    }

    IComponent * PlayerComponent::copy(int entity) {
        PlayerComponent* pc = new PlayerComponent(m_game, 0);
        return pc;
    }

    void PlayerComponent::handleInput() {
        bool keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool keySpace = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        auto* mc = m_game.getWorld()->getEntity(0)->getComponent<MovementComponent>();
        switch(m_state) {
            case IDLE:
                if(!(keyA && keyD) || !(keyA || keyD)) {
                    if(keyA) {
                        mc->setVelocityX(-3.0F);
                    } else if(keyD) {
                        mc->setVelocityX(3.0F);
                    }
                }
                if(keySpace) {
                    m_state = JUMPING;
                    mc->setVelocityY(15.0F);
                }
                break;
            case JUMPING:
                if(!(keyA && keyD) || !(keyA || keyD)) {
                    if(keyA) {
                        mc->setVelocityX(-3.0F);
                    } else if(keyD) {
                        mc->setVelocityX(3.0F);
                    }
                }
                if(mc->getNewCoord().y == 0)
                    m_state = IDLE;
                break;
        }

    }
}