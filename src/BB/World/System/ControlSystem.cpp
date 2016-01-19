#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    ControlSystem::ControlSystem(GameStateGame& game) : m_game(game) {

    }

    void ControlSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(PlayerComponent))] = std::unique_ptr<CList>(new CList());
    }

    void ControlSystem::createComponent(luabridge::LuaRef& luaE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaPC = luaComponents["PlayerComponent"];
        if(luaPC.isNil()) return;
        auto* pc = new PlayerComponent();
        pc->m_state = pc->IDLE;
        pc->m_facingLeft = true;
        pc->m_movingLeft = true;
        list[std::type_index(typeid(PlayerComponent))] = std::unique_ptr<PlayerComponent>(pc);
    }

    void ControlSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* component = list[std::type_index(typeid(PlayerComponent))].get();
        if(!component) return;
        auto* pc = new PlayerComponent(*dynamic_cast<PlayerComponent*>(component));
        entity->addComponent(std::type_index(typeid(PlayerComponent)), pc);
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
        sf::Vector2f mouseCoord = m_game.getWindowHandler()->getWindow().mapPixelToCoords(mousePos);
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        auto& cList = m_game.getWorld().getField()->getComponentList<PlayerComponent>()->m_list;
        for(auto& c : cList) {
            auto& pc = *dynamic_cast<PlayerComponent*>(c.second.get());
            auto& mc = *m_game.getWorld().getField()->getComponent<MovementComponent>(c.first);
            auto gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(c.first);
            auto* pE = m_game.getWorld().getField()->getEntity(c.first);
            sf::Vector2f playerPos = gs.mapCoordsToPixel(pE->getCoord());
            if(mouseCoord.x > playerPos.x + gs.getTileSize() / 2 && pc.m_facingLeft == true) {
                if(pc.m_state == PlayerComponent::IDLE)
                    gs.setAnimation(gc, "idleR");
                else if(pc.m_state == PlayerComponent::WALKING)
                    gs.setAnimation(gc, "walkR");
                else if(pc.m_state == PlayerComponent::CROUCHING)
                    gs.setAnimation(gc, "crouchR");
                pc.m_facingLeft = false;
            } else if(mouseCoord.x < playerPos.x + gs.getTileSize() / 2 && pc.m_facingLeft == false) {
                if(pc.m_state == PlayerComponent::IDLE)
                    gs.setAnimation(gc, "idleL");
                else if(pc.m_state == PlayerComponent::WALKING)
                    gs.setAnimation(gc, "walkL");
                else if(pc.m_state == PlayerComponent::CROUCHING)
                    gs.setAnimation(gc, "crouchL");
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
                            gs.setAnimation(gc, "walkL");
                        else
                            gs.setAnimation(gc, "walkR");
                    }
                    if(keyShift) {
                        pc.m_state = PlayerComponent::CROUCHING;
                        mc.m_velocities.x = 0.0F;
                        if(pc.m_facingLeft)
                            gs.setAnimation(gc, "crouchL");
                        else
                            gs.setAnimation(gc, "crouchR");
                    } else if(keySpace || keyW) {
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
                            gs.setAnimation(gc, "idleL");
                        else
                            gs.setAnimation(gc, "idleR");
                    }
                    if(keyShift) {
                        pc.m_state = PlayerComponent::CROUCHING;
                        mc.m_velocities.x = 0.0F;
                        if(pc.m_facingLeft)
                            gs.setAnimation(gc, "crouchL");
                        else
                            gs.setAnimation(gc, "crouchR");
                    } else if(keySpace || keyW) {
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
                            gs.setAnimation(gc, "idleL");
                        else
                            gs.setAnimation(gc, "walkL");
                    } else {
                        if(mc.m_velocities.x == 0)
                            gs.setAnimation(gc, "idleR");
                        else
                            gs.setAnimation(gc, "walkR");
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
                        gs.setAnimation(gc, "crouchL");
                    else
                        gs.setAnimation(gc, "crouchR");
                    if(!keyShift)
                        if(mc.m_velocities.x == 0) {
                            pc.m_state = PlayerComponent::IDLE;
                            if(pc.m_facingLeft)
                                gs.setAnimation(gc, "idleL");
                            else
                                gs.setAnimation(gc, "idleR");
                        } else {
                            pc.m_state = PlayerComponent::WALKING;
                            if(pc.m_facingLeft)
                                gs.setAnimation(gc, "walkL");
                            else
                                gs.setAnimation(gc, "walkR");
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
        if(windowEvent.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
            sf::Vector2f mousePixel = m_game.getWindowHandler()->getWindow().mapPixelToCoords(mousePos);
            sf::Vector2f mouseCoord = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords(mousePixel);
            auto& list = m_game.getWorld().getField()->getComponentList<HealthComponent>()->m_list;
            for(auto& hc : list) {
                auto* entity = m_game.getWorld().getField()->getEntity(hc.first);
                if(m_game.getWorld().getSystem<PhysicsSystem>().contain(entity, mouseCoord)) {
                    entity->addComponent(std::type_index(typeid(DamageComponent)), new DamageComponent(1));
                }
            }
        }
    }
}