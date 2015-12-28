#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    ControlSystem::ControlSystem(GameStateGame& game) : m_game(game) {

    }

    IComponent* ControlSystem::createComponent(luabridge::LuaRef& luaE) {
        auto* pc = new PlayerComponent();
        pc->m_state = pc->IDLE;
        pc->m_facingLeft = true;
        pc->m_movingLeft = true;
        return pc;
    }

    IComponent* ControlSystem::createComponent(IComponent* component, rapidjson::Value& jsonE) {
        auto* pc = new PlayerComponent(*dynamic_cast<PlayerComponent*>(component));
        return pc;
    }

    void ControlSystem::handleInput() {
        bool keyW = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyS = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        bool keySpace = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        bool keyControl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        bool keyShift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        bool mouseLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
        auto& cList = *m_game.getWorld().getField()->getComponentList<PlayerComponent>();
        for(auto& c : cList) {
            auto& pc = *dynamic_cast<PlayerComponent*>(c.second);
            auto& mc = *m_game.getWorld().getField()->getComponent<MovementComponent>(c.first);
            auto* pE = m_game.getWorld().getField()->getEntity(c.first);
            sf::Vector2f playerPos = m_game.getWorld().getGraphicsSystem().mapCoordsToPixel(pE->getCoord());
            if(mousePos.x > playerPos.x)
                pc.m_facingLeft = false;
            else if(mousePos.x < playerPos.x)
                pc.m_facingLeft = true;
            switch(pc.m_state) {
                case PlayerComponent::IDLE:
                case PlayerComponent::WALKING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            pc.m_movingLeft = true;
                            pc.m_state = PlayerComponent::WALKING;
                            mc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            pc.m_movingLeft = false;
                            pc.m_state = PlayerComponent::WALKING;
                            mc.m_velocities.x = 3.0F;
                        }
                    } else {
                        pc.m_state = PlayerComponent::IDLE;
                        mc.m_velocities.x = 0.0F;
                    }
                    if(keySpace || keyW) {
                        pc.m_state = PlayerComponent::JUMPING;
                        mc.m_velocities.y = 15.0F;
                        mc.m_isOnGround = false;
                    }
                    break;
                case PlayerComponent::JUMPING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            pc.m_movingLeft = true;
                            mc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            pc.m_movingLeft = false;
                            mc.m_velocities.x = 3.0F;
                        }
                    } else {
                        mc.m_velocities.x = 0.0F;
                    }
                    if(mc.m_isOnGround)
                        pc.m_state = PlayerComponent::IDLE;
                    break;
                case PlayerComponent::CROUCHING:
                    break;
            }
        }
    }

    void ControlSystem::handleInput(sf::Event& windowEvent) {

    }
}