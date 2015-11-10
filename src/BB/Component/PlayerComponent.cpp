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
        m_facingLeft = true;
    }

    IComponent * PlayerComponent::copy(int entity) {
        PlayerComponent* pc = new PlayerComponent(m_game, 0);
        return pc;
    }

    void PlayerComponent::handleInput() {
        bool keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool keyW = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool keyS = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool keySpace = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        bool keyControl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        bool keyShift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        bool mouseLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
        sf::Vector2f playerPos = {(m_game.getWorld()->getEntity(0)->getCoord().x
            - m_game.getGraphicsHandler()->getViewCoord().x) * 64.0F + 32.0F
            + m_game.getWindowHandler()->getWindow().getSize().x / 2,
            m_game.getWindowHandler()->getWindow().getSize().y
            - (m_game.getWorld()->getEntity(0)->getCoord().y
                - m_game.getGraphicsHandler()->getViewCoord().y + 1) * 64.0F + 32.0F};
        auto* gc = m_game.getWorld()->getEntity(0)->getComponent<GraphicsComponent>();
        auto* mc = m_game.getWorld()->getEntity(0)->getComponent<MovementComponent>();
        sf::Sprite* sprite;
        gc->getDrawable("base", sprite);
        if(mousePos.x > playerPos.x) {
            m_facingLeft = false;
        } else if(mousePos.x < playerPos.x){
            m_facingLeft = true;
        }
        switch(m_state) {
            case IDLE:
                if(keySpace || keyW) {
                    m_state = JUMPING;
                    mc->setVelocityY(15.0F);
                } else if(keyShift || keyS) {
                    m_state = CROUCHING;
                    break;
                }
                if(!(keyA && keyD) || !(keyA || keyD)) {
                    if(keyA) {
                        if(keyControl)
                            mc->setVelocityX(-6.0F);
                        else
                            mc->setVelocityX(-3.0F);
                    } else if(keyD) {
                        if(keyControl)
                            mc->setVelocityX(6.0F);
                        else
                            mc->setVelocityX(3.0F);
                    }
                }
                if(mouseLeft) {
                    if(m_facingLeft) {
                        sprite->setTextureRect({64, 16, 16, 16});
                    } else {
                        sprite->setTextureRect({64, 0, 16, 16});
                    }
                } else {
                    if(m_facingLeft) {
                        sprite->setTextureRect({0, 16, 16, 16});
                    } else {
                        sprite->setTextureRect({0, 0, 16, 16});
                    }
                }
                break;
            case JUMPING:
                if(!(keyA && keyD) || !(keyA || keyD)) {
                    if(keyA) {
                        if(keyControl)
                            mc->setVelocityX(-6.0F);
                        else
                            mc->setVelocityX(-3.0F);
                    } else if(keyD) {
                        if(keyControl)
                            mc->setVelocityX(6.0F);
                        else
                            mc->setVelocityX(3.0F);
                    }
                }
                if(m_game.getWorld()->getEntity(0)->getComponent<MovementComponent>()->isOnGround())
                    m_state = IDLE;
                if(mouseLeft) {
                    if(m_facingLeft) {
                        sprite->setTextureRect({64, 16, 16, 16});
                    } else {
                        sprite->setTextureRect({64, 0, 16, 16});
                    }
                } else {
                    if(m_facingLeft) {
                        sprite->setTextureRect({0, 16, 16, 16});
                    } else {
                        sprite->setTextureRect({0, 0, 16, 16});
                    }
                }
                break;
            case CROUCHING:
                if(!(keyA && keyD) || !(keyA || keyD)) {
                    if(keyA) {
                        mc->setVelocityX(-1.5F);
                    } else if(keyD) {
                        mc->setVelocityX(1.5F);
                    }
                }
                if(!(keyShift || keyS))
                    m_state = IDLE;
                if(m_facingLeft) {
                    sprite->setTextureRect({48, 16, 16, 16});
                } else {
                    sprite->setTextureRect({48, 0, 16, 16});
                }
                break;
        }

    }
}