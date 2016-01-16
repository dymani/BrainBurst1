#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    ControlSystem::ControlSystem(GameStateGame& game) : m_game(game) {

    }

    void ControlSystem::createList(std::map<std::type_index, std::map<int, IComponent*>*>& lists) {
        lists[std::type_index(typeid(PlayerComponent))] = new std::map<int, IComponent*>;
    }

    void ControlSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index,
        IComponent*>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaPC = luaComponents["PlayerComponent"];
        if(luaPC.isNil()) return;
        auto* pc = new PlayerComponent();
        pc->m_state = pc->IDLE;
        pc->m_facingLeft = true;
        pc->m_movingLeft = true;
        list[std::type_index(typeid(PlayerComponent))] = pc;
    }

    void ControlSystem::createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
        Entity* entity) {
        auto* component = list[std::type_index(typeid(PlayerComponent))];
        if(!component) return;
        auto* pc = new PlayerComponent(*dynamic_cast<PlayerComponent*>(component));
        entity->addComponent(m_game, std::type_index(typeid(PlayerComponent)), pc);
    }

    void ControlSystem::handleInput() {
        bool keyW = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        bool keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        bool keyS = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        bool keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

        bool keyW2 = sf::Keyboard::isKeyPressed(sf::Keyboard::T);
        bool keyA2 = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
        bool keyS2 = sf::Keyboard::isKeyPressed(sf::Keyboard::G);
        bool keyD2 = sf::Keyboard::isKeyPressed(sf::Keyboard::H);

        bool keySpace = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        bool keyControl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        bool keyShift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        bool mouseLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
        mousePos = sf::Vector2i(m_game.getWindowHandler()->getWindow().mapPixelToCoords(mousePos));
        auto& cList = *m_game.getWorld().getField()->getComponentList<PlayerComponent>();
        for(auto& c : cList) {
            auto& pc = *dynamic_cast<PlayerComponent*>(c.second);
            auto& mc = *m_game.getWorld().getField()->getComponent<MovementComponent>(c.first);
            auto gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(c.first);
            auto* pE = m_game.getWorld().getField()->getEntity(c.first);
            sf::Vector2f playerPos = m_game.getWorld().getSystem<GraphicsSystem>().mapCoordsToPixel(pE->getCoord());
            if(mousePos.x > playerPos.x && pc.m_facingLeft == true) {
                if(pc.m_state == PlayerComponent::IDLE)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleR");
                else if(pc.m_state == PlayerComponent::WALKING)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkR");
                else if(pc.m_state == PlayerComponent::CROUCHING)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchR");
                pc.m_facingLeft = false;
            } else if(mousePos.x < playerPos.x && pc.m_facingLeft == false) {
                if(pc.m_state == PlayerComponent::IDLE)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleL");
                else if(pc.m_state == PlayerComponent::WALKING)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkL");
                else if(pc.m_state == PlayerComponent::CROUCHING)
                    m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchL");
                pc.m_facingLeft = true;
            }
            switch(pc.m_state) {
                case PlayerComponent::IDLE:
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
                        if(pc.m_facingLeft)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkL");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkR");
                    }
                    if(keyShift) {
                        pc.m_state = PlayerComponent::CROUCHING;
                        mc.m_velocities.x = 0.0F;
                        if(pc.m_facingLeft)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchL");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchR");
                    }else if(keySpace || keyW) {
                        pc.m_state = PlayerComponent::JUMPING;
                        mc.m_velocities.y = 15.0F;
                        mc.m_isOnGround = false;
                    }
                    break;
                case PlayerComponent::WALKING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            pc.m_movingLeft = true;
                            mc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            pc.m_movingLeft = false;
                            mc.m_velocities.x = 3.0F;
                        }
                    } else {
                        pc.m_state = PlayerComponent::IDLE;
                        mc.m_velocities.x = 0.0F;
                        if(pc.m_facingLeft)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleL");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleR");
                    }
                    if(keyShift) {
                        pc.m_state = PlayerComponent::CROUCHING;
                        mc.m_velocities.x = 0.0F;
                        if(pc.m_facingLeft)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchL");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchR");
                    }else if(keySpace || keyW) {
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
                    if(pc.m_facingLeft) {
                        if(mc.m_velocities.x == 0)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleL");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkL");
                    } else {
                        if(mc.m_velocities.x == 0)
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleR");
                        else
                            m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkR");
                    }
                    if(mc.m_isOnGround)
                        if(mc.m_velocities.x == 0)
                            pc.m_state = PlayerComponent::IDLE;
                        else
                            pc.m_state = PlayerComponent::WALKING;
                    break;
                case PlayerComponent::CROUCHING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            pc.m_movingLeft = true;
                            mc.m_velocities.x = -1.5F;
                        } else if(keyD) {
                            pc.m_movingLeft = false;
                            mc.m_velocities.x = 1.5F;
                        }
                    } else {
                        mc.m_velocities.x = 0.0F;
                    }
                    if(pc.m_facingLeft)
                        m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchL");
                    else
                        m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "crouchR");
                    if(!keyShift)
                        if(mc.m_velocities.x == 0) {
                            pc.m_state = PlayerComponent::IDLE;
                            if(pc.m_facingLeft)
                                m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleL");
                            else
                                m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "idleR");
                        } else {
                            pc.m_state = PlayerComponent::WALKING;
                            if(pc.m_facingLeft)
                                m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkL");
                            else
                                m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "walkR");
                        }
                    break;
            }
            //if(keyW2 || keyS2) {
            //    if(keyW2) mc.m_velocities.y = 3.0F;
            //    if(keyS2) mc.m_velocities.y = -3.0F;
            //} else {
            //    mc.m_velocities.y = 0;
            //}
            //if(keyA2 || keyD2) {
            //    if(keyA2) mc.m_velocities.x = -3.0F;
            //    if(keyD2) mc.m_velocities.x = 3.0F;
            //} else {
            //    mc.m_velocities.x = 0;
            //}
        }
    }

    void ControlSystem::handleInput(sf::Event& windowEvent) {

    }
}