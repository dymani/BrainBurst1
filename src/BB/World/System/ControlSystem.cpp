#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    ControlSystem::ControlSystem(GameStateGame& game) : m_game(game) {

    }

    void ControlSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(ControlComponent))] = std::unique_ptr<CList>(new CList());
    }

    void ControlSystem::createComponent(luabridge::LuaRef& luaE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaCC = luaComponents["ControlComponent"];
        if(luaCC.isNil()) return;
        auto* cc = new ControlComponent();
        LuaRef luaControl = luaCC["control"];
        cc->m_control = luaControl.cast<bool>();
        cc->m_state = cc->IDLE;
        cc->m_facingLeft = true;
        cc->m_movingLeft = true;
        list[std::type_index(typeid(ControlComponent))] = std::unique_ptr<ControlComponent>(cc);
    }

    void ControlSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* component = list[std::type_index(typeid(ControlComponent))].get();
        if(!component) return;
        auto* cc = new ControlComponent(*dynamic_cast<ControlComponent*>(component));
        entity->addComponent(std::type_index(typeid(ControlComponent)), cc);
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
        auto& cList = m_game.getWorld().getField()->getComponentList<ControlComponent>()->m_list;
        for(auto& c : cList) {
            auto& cc = *dynamic_cast<ControlComponent*>(c.second.get());
            if(!cc.m_control) continue;
            auto& pc = *m_game.getWorld().getField()->getComponent<PhysicsComponent>(c.first);
            auto gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(c.first);
            auto* pE = m_game.getWorld().getField()->getEntity(c.first);
            sf::Vector2f playerPos = gs.mapCoordsToPixel(pE->getCoord());
            if(mouseCoord.x > playerPos.x + gs.getTileSize() / 2 && cc.m_facingLeft == true) {
                if(cc.m_state == ControlComponent::IDLE)
                    gs.setAnimation(gc, "idleR");
                else if(cc.m_state == ControlComponent::WALKING)
                    gs.setAnimation(gc, "walkR");
                else if(cc.m_state == ControlComponent::CROUCHING)
                    gs.setAnimation(gc, "crouchR");
                cc.m_facingLeft = false;
            } else if(mouseCoord.x < playerPos.x + gs.getTileSize() / 2 && cc.m_facingLeft == false) {
                if(cc.m_state == ControlComponent::IDLE)
                    gs.setAnimation(gc, "idleL");
                else if(cc.m_state == ControlComponent::WALKING)
                    gs.setAnimation(gc, "walkL");
                else if(cc.m_state == ControlComponent::CROUCHING)
                    gs.setAnimation(gc, "crouchL");
                cc.m_facingLeft = true;
            }
            switch(cc.m_state) {
                case ControlComponent::IDLE:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            cc.m_movingLeft = true;
                            cc.m_state = ControlComponent::WALKING;
                            pc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            cc.m_state = ControlComponent::WALKING;
                            pc.m_velocities.x = 3.0F;
                        }
                        if(cc.m_facingLeft)
                            gs.setAnimation(gc, "walkL");
                        else
                            gs.setAnimation(gc, "walkR");
                    }
                    if(keyShift || keyS) {
                        cc.m_state = ControlComponent::CROUCHING;
                        pc.m_velocities.x = 0.0F;
                        if(cc.m_facingLeft)
                            gs.setAnimation(gc, "crouchL");
                        else
                            gs.setAnimation(gc, "crouchR");
                    } else if(keySpace || keyW) {
                        cc.m_state = ControlComponent::JUMPING;
                        pc.m_velocities.y = 15.0F;
                        pc.m_isOnGround = false;
                    }
                    break;
                case ControlComponent::WALKING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            cc.m_movingLeft = true;
                            pc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            pc.m_velocities.x = 3.0F;
                        }
                    } else {
                        cc.m_state = ControlComponent::IDLE;
                        pc.m_velocities.x = 0.0F;
                        if(cc.m_facingLeft)
                            gs.setAnimation(gc, "idleL");
                        else
                            gs.setAnimation(gc, "idleR");
                    }
                    if(keyShift || keyS) {
                        cc.m_state = ControlComponent::CROUCHING;
                        pc.m_velocities.x = 0.0F;
                        if(cc.m_facingLeft)
                            gs.setAnimation(gc, "crouchL");
                        else
                            gs.setAnimation(gc, "crouchR");
                    } else if(keySpace || keyW) {
                        cc.m_state = ControlComponent::JUMPING;
                        pc.m_velocities.y = 15.0F;
                        pc.m_isOnGround = false;
                    }
                    break;
                case ControlComponent::JUMPING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            cc.m_movingLeft = true;
                            pc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            pc.m_velocities.x = 3.0F;
                        }
                    } else {
                        pc.m_velocities.x = 0.0F;
                    }
                    if(cc.m_facingLeft) {
                        if(pc.m_velocities.x == 0)
                            gs.setAnimation(gc, "idleL");
                        else
                            gs.setAnimation(gc, "walkL");
                    } else {
                        if(pc.m_velocities.x == 0)
                            gs.setAnimation(gc, "idleR");
                        else
                            gs.setAnimation(gc, "walkR");
                    }
                    if(pc.m_isOnGround)
                        if(pc.m_velocities.x == 0)
                            cc.m_state = ControlComponent::IDLE;
                        else
                            cc.m_state = ControlComponent::WALKING;
                    break;
                case ControlComponent::CROUCHING:
                    if(keyA ^ keyD) {
                        if(keyA) {
                            cc.m_movingLeft = true;
                            pc.m_velocities.x = -1.5F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            pc.m_velocities.x = 1.5F;
                        }
                    } else {
                        pc.m_velocities.x = 0.0F;
                    }
                    if(cc.m_facingLeft)
                        gs.setAnimation(gc, "crouchL");
                    else
                        gs.setAnimation(gc, "crouchR");
                    if(!(keyShift || keyS))
                        if(pc.m_velocities.x == 0) {
                            cc.m_state = ControlComponent::IDLE;
                            if(cc.m_facingLeft)
                                gs.setAnimation(gc, "idleL");
                            else
                                gs.setAnimation(gc, "idleR");
                        } else {
                            cc.m_state = ControlComponent::WALKING;
                            if(cc.m_facingLeft)
                                gs.setAnimation(gc, "walkL");
                            else
                                gs.setAnimation(gc, "walkR");
                        }
                    break;
            }
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
                    m_game.getWorld().getSystem<HealthSystem>().addDamage(
                        dynamic_cast<HealthComponent*>(hc.second.get()), 1);
                }
            }
        }
    }
}