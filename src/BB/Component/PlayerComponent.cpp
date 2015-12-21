#include "BB/Component/PlayerComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/MovementComponent.h"
#include "BB/Component/BreakableComponent.h"

namespace bb {
    PlayerComponent* PlayerComponent::create(GameStateGame& game, luabridge::lua_State* L,
        luabridge::LuaRef& luaPC) {
        PlayerComponent* pc = new PlayerComponent(game, -1);
        return pc;
    }

    PlayerComponent::PlayerComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
        m_state = IDLE;
        m_walk = STAND;
        m_sprintCount = 0;
        m_speed = 3.0F;
        m_facingLeft = true;
    }

    IComponent* PlayerComponent::copy(rapidjson::Value& value) {
        return copy(0);
    }

    IComponent* PlayerComponent::copy(int entity) {
        PlayerComponent* pc = new PlayerComponent(m_game, entity);
        pc->m_state = m_state;
        pc->m_facingLeft = m_facingLeft;
        pc->m_speed = m_speed;
        pc->m_walk = m_walk;
        pc->m_sprintCount = m_sprintCount;
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
        } else if(mousePos.x < playerPos.x) {
            m_facingLeft = true;
        }
        if(m_walk == L_PRE || m_walk == R_PRE) {
            if(m_walk == L_PRE && keyA) {
                m_walk = L_SPRINTING;
                m_speed = 6.0F;
                m_sprintCount = 0;
            } else if(m_walk == R_PRE && keyD) {
                m_walk = R_SPRINTING;
                m_speed = 6.0F;
                m_sprintCount = 0;
            } else {
                m_sprintCount++;
                if(m_sprintCount >= 50) {
                    m_sprintCount = 0;
                    m_walk = STAND;
                }
            }
        }
        if(mc->getVelocity().x == 0)
            if(m_speed == 6.0F)
                m_speed = 3.0F;
        switch(m_state) {
            case IDLE:
                if(keySpace || keyW) {
                    m_state = JUMPING;
                    mc->setVelocityY(15.0F);
                } else if(keyShift || keyS) {
                    m_state = CROUCHING;
                    m_speed = 1.5F;
                    break;
                }
                if((keyA && keyD) || (!keyA && !keyD)) {
                    m_speed = 3.0F;
                    if(m_walk == L_SPRINTING || m_walk == L_WALKING) {
                        m_walk = L_PRE;
                        m_sprintCount = 0;
                    } else if(m_walk == R_SPRINTING || m_walk == R_WALKING) {
                        m_walk = R_PRE;
                        m_sprintCount = 0;
                    }
                } else {
                    if(keyA) {
                        if(keyControl) {
                            m_speed = 6.0F;
                            m_walk = L_SPRINTING;
                        } else {
                            m_walk = L_WALKING;
                        }
                        mc->setVelocityX(0 - m_speed);
                    } else if(keyD) {
                        if(keyControl) {
                            m_speed = 6.0F;
                            m_walk = R_SPRINTING;
                        } else {
                            m_walk = R_WALKING;
                        }
                        mc->setVelocityX(m_speed);
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
                if((keyA && keyD) || (!keyA && !keyD)) {
                    m_speed = 3.0F;
                    if(m_walk == L_SPRINTING || m_walk == L_WALKING) {
                        m_walk = L_PRE;
                        m_sprintCount = 0;
                    } else if(m_walk == R_SPRINTING || m_walk == R_WALKING) {
                        m_walk = R_PRE;
                        m_sprintCount = 0;
                    }
                } else {
                    if(keyA) {
                        m_walk = L_WALKING;
                        mc->setVelocityX(0 - m_speed);
                    } else if(keyD) {
                        m_walk = R_WALKING;
                        mc->setVelocityX(m_speed);
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
                if(!(keyA && keyD) && !(!keyA && !keyD)) {
                    if(keyA) {
                        mc->setVelocityX(0 - m_speed);
                    } else if(keyD) {
                        mc->setVelocityX(m_speed);
                    }
                }
                if(!(keyShift || keyS)) {
                    m_state = IDLE;
                    m_speed = 3.0F;
                }
                if(m_facingLeft) {
                    sprite->setTextureRect({48, 16, 16, 16});
                } else {
                    sprite->setTextureRect({48, 0, 16, 16});
                }
                break;
        }

    }

    void PlayerComponent::handleInput(sf::Event windowEvent) {
        if(windowEvent.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mouse = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
            sf::Vector2f coord = m_game.getWorld()->getEntity(m_entity)->getCoord();
            sf::Vector2f viewCoord = m_game.getGraphicsHandler()->getViewCoord();
            sf::Vector2u windowSize = m_game.getWindowHandler()->getWindow().getSize();
            float mouseX = float(mouse.x) / 64.0F - float(windowSize.x) / 64.0F / 2.0F + viewCoord.x;
            float mouseY = float(windowSize.y) / 64.0F - float(mouse.y) / 64.0F - 1.0F + viewCoord.y;
            if(((mouseX > coord.x && mouseX - coord.x <= 3)
                || (mouseX < coord.x && coord.x - mouseX <= 3))
                && ((mouseY > coord.y && mouseY - coord.y <= 3)
                    || (mouseY < coord.y && coord.y - mouseY <= 3))) {
                for(auto& entity : m_game.getWorld()->getEntities()) {
                    if(int(entity.second->getCoord().x) == int(mouseX)
                        && int(entity.second->getCoord().y) == int(mouseY)) {
                        if(entity.second->getComponent<BreakableComponent>()) {
                            entity.second->getComponent<BreakableComponent>()->addDurability(-1);
                        }
                    }
                }
            }
        }
    }

    bool PlayerComponent::getUpdate() {
        return false;
    }
}