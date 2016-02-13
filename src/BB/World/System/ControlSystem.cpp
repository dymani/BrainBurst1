#include "BB/World/System/ControlSystem.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    void ControlSystem::registerState(luabridge::lua_State* L) {
        luabridge::getGlobalNamespace(L)
            .beginClass<LuaInput>("LuaInput")
            .addData("keyW", &LuaInput::keyW, false)
            .addData("keyA", &LuaInput::keyA, false)
            .addData("keyS", &LuaInput::keyS, false)
            .addData("keyD", &LuaInput::keyD, false)
            .addData("keySpace", &LuaInput::keySpace, false)
            .addData("keyShift", &LuaInput::keyShift, false)
            .addData("keyCtrl", &LuaInput::keyCtrl, false)
            .addData("mouseLeft", &LuaInput::mouseLeft, false)
            .addData("mouseX", &LuaInput::mouseX, false)
            .addData("mouseY", &LuaInput::mouseY, false)
            .endClass();
    }

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
        LuaRef luaScriptName = luaCC["scriptName"];
        cc->m_control = luaControl.cast<bool>();
        cc->m_scriptName = luaScriptName.cast<std::string>();
        if(cc->m_scriptName == "") {
            cc->m_script = false;
        } else {
            if(m_scripts.find(cc->m_scriptName) == m_scripts.end()) {
                using namespace luabridge;
                auto* L = m_game.getLuaState();
                std::string file = "assets/data/world/scripts/" + cc->m_scriptName + ".lua";
                if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
                    LogHandler::log<Field>(ERR, "Script \"" + file + "\" not found");
                    cc->m_script = false;
                } else {
                    cc->m_script = true;
                    luabridge::LuaRef& luaOnInput = getGlobal(L, "onInput");
                    m_scripts[cc->m_scriptName] = std::make_shared<LuaRef>(luaOnInput);
                }
            } else {
                cc->m_script = true;
            }
        }
        list[std::type_index(typeid(ControlComponent))] = std::unique_ptr<ControlComponent>(cc);
    }

    void ControlSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* component = list[std::type_index(typeid(ControlComponent))].get();
        if(!component) return;
        auto* cc = new ControlComponent(*dynamic_cast<ControlComponent*>(component));
        if(cc->m_control) m_player = entity->getId();
        entity->addComponent(std::type_index(typeid(ControlComponent)), cc);
    }

    void ControlSystem::createComponent(std::map<std::type_index, std::unique_ptr<IComponent>>& list,
        Entity* entity) {
        auto* component = list[std::type_index(typeid(ControlComponent))].get();
        if(!component) return;
        auto* cc = new ControlComponent(*dynamic_cast<ControlComponent*>(component));
        if(cc->m_control) m_player = entity->getId();
        entity->addComponent(std::type_index(typeid(ControlComponent)), cc);
    }

    void ControlSystem::handleInput() {
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
        sf::Vector2f mousePos2 = m_game.getWindowHandler()->getWindow().mapPixelToCoords(mousePos);
        sf::Vector2f mouseCoord = gs.mapPixelToCoords(mousePos2);
        LuaInput input;
        input.keyW = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
        input.keyA = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        input.keyS = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
        input.keyD = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        input.keySpace = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
        input.keyShift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        input.keyCtrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        input.mouseLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        input.mouseX = mouseCoord.x;
        input.mouseY = mouseCoord.y;

        auto& cList = m_game.getWorld().getField()->getComponentList<ControlComponent>()->m_list;
        for(auto& c : cList) {
            auto& cc = *dynamic_cast<ControlComponent*>(c.second.get());
            if(!cc.m_script) continue;
            if(cc.m_control) {
                try {
                    if((*m_scripts[cc.m_scriptName])(new LuaEntity(m_game, c.first), input).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<ControlSystem>(ERR, "LuaException: ");
                    std::cout << "                " << e.what() << std::endl;
                }
            } else {
                try {
                    if((*m_scripts[cc.m_scriptName])(new LuaEntity(m_game, c.first)).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<ControlSystem>(ERR, "LuaException: ");
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
            if(windowEvent.mouseButton.button == sf::Mouse::Left) {
                /*auto& list = m_game.getWorld().getField()->getComponentList<HealthComponent>()->m_list;
                for(auto& hc : list) {
                    auto* entity = m_game.getWorld().getField()->getEntity(hc.first);
                    if(m_game.getWorld().getSystem<PhysicsSystem>().contain(entity, mouseCoord)) {
                        m_game.getWorld().getSystem<HealthSystem>().hit(hc.first);
                    }
                }*/
            } else if(windowEvent.mouseButton.button == sf::Mouse::Right) {
            }
        } else if(windowEvent.type == sf::Event::KeyPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_game.getWindowHandler()->getWindow());
            sf::Vector2f mousePixel = m_game.getWindowHandler()->getWindow().mapPixelToCoords(mousePos);
            sf::Vector2f mouseCoord = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords(mousePixel);
            if(windowEvent.key.code == sf::Keyboard::Num1) {
                /*if(m_game.getWorld().getField()->getEntity(m_player)->getCoord().x > mouseCoord.x)
                    m_game.getWorld().getField()->createEntity("FireL", mouseCoord);
                else
                    m_game.getWorld().getField()->createEntity("FireR", mouseCoord);*/
            }
        }
    }

    int ControlSystem::getPlayerId() {
        return m_player;
    }
}