#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/LuaEntity.h"

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
        LuaRef luaOnInput = luaCC["onInput"];
        cc->m_control = luaControl.cast<bool>();
        cc->m_state = cc->IDLE;
        cc->m_facingLeft = true;
        cc->m_movingLeft = true;
        cc->m_onInput = std::make_shared<LuaRef>(luaOnInput);
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
            bool update = false;
            auto& cc = *dynamic_cast<ControlComponent*>(c.second.get());
            if(!cc.m_control) continue;
            auto& pc = *m_game.getWorld().getField()->getComponent<PhysicsComponent>(c.first);
            auto gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(c.first);
            auto* pE = m_game.getWorld().getField()->getEntity(c.first);
            sf::Vector2f playerPos = gs.mapCoordsToPixel(pE->getCoord());
            if(mouseCoord.x > playerPos.x + gs.getTileSize() / 2 && cc.m_facingLeft == true) {
                update = true;
                cc.m_facingLeft = false;
            } else if(mouseCoord.x < playerPos.x + gs.getTileSize() / 2 && cc.m_facingLeft == false) {
                update = true;
                cc.m_facingLeft = true;
            }
            switch(cc.m_state) {
                case ControlComponent::IDLE:
                    if(keyA ^ keyD) {
                        update = true;
                        if(keyA) {
                            cc.m_movingLeft = true;
                            cc.m_state = ControlComponent::WALKING;
                            pc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            cc.m_state = ControlComponent::WALKING;
                            pc.m_velocities.x = 3.0F;
                        }
                    }
                    if(keyShift || keyS) {
                        update = true;
                        cc.m_state = ControlComponent::CROUCHING;
                        pc.m_velocities.x = 0.0F;
                    } else if(keySpace || keyW) {
                        update = true;
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
                        update = true;
                        cc.m_state = ControlComponent::IDLE;
                        pc.m_velocities.x = 0.0F;
                    }
                    if(keyShift || keyS) {
                        update = true;
                        cc.m_state = ControlComponent::CROUCHING;
                        pc.m_velocities.x = 0.0F;
                    } else if(keySpace || keyW) {
                        update = true;
                        cc.m_state = ControlComponent::JUMPING;
                        pc.m_velocities.y = 15.0F;
                        pc.m_isOnGround = false;
                    }
                    break;
                case ControlComponent::JUMPING:
                    if(keyA ^ keyD) {
                        if(pc.m_velocities.x == 0)
                            update = true;
                        if(keyA) {
                            cc.m_movingLeft = true;
                            pc.m_velocities.x = -3.0F;
                        } else if(keyD) {
                            cc.m_movingLeft = false;
                            pc.m_velocities.x = 3.0F;
                        }
                    } else {
                        if(pc.m_velocities.x != 0)
                            update = true;
                        pc.m_velocities.x = 0.0F;
                    }
                    if(pc.m_isOnGround) {
                        update = true;
                        if(pc.m_velocities.x == 0)
                            cc.m_state = ControlComponent::IDLE;
                        else
                            cc.m_state = ControlComponent::WALKING;
                    }
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
                    if(!(keyShift || keyS)) {
                        update = true;
                        if(pc.m_velocities.x == 0) {
                            cc.m_state = ControlComponent::IDLE;
                        } else {
                            cc.m_state = ControlComponent::WALKING;
                        }
                    }
                    break;
            }
            if(update) {
                try {
                    if((*cc.m_onInput)(new LuaEntity(m_game, c.first)).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<HealthSystem>(ERR, "LuaException: ");
                    std::cout << "                " << e.what() << std::endl;
                }
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
                    m_game.getWorld().getSystem<HealthSystem>().hit(hc.first);
                }
            }
        }
    }

    int ControlSystem::getState(ControlComponent* cc) {
        return int(cc->m_state);
    }

    bool ControlSystem::isFacingLeft(ControlComponent* cc) {
        return cc->m_facingLeft;
    }

    bool ControlSystem::isMovingLeft(ControlComponent* cc) {
        return cc->m_movingLeft;
    }
}