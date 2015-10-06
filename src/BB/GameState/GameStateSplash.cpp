#include "BB/GameState/GameStateSplash.h"
#include "BB/Game.h"
#include "BB/Component/UpdateComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GameStateSplash::GameStateSplash(Game& game, ResourceHandler* resourceHandler,
        luabridge::lua_State* L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_audioHandler = new AudioHandler(*m_resourceHandler);
        m_windowHandler = new WindowHandler();
        m_graphicsHandler = new GraphicsHandler(*m_windowHandler, m_entities);
        m_guiHandler = new GuiHandler(*m_windowHandler, m_entities);
        m_updateHandler = new UpdateHandler(m_entities, this);
        m_scriptHandler = new ScriptHandler(*m_resourceHandler, m_updateHandler);
        this->L = L;
        m_windowHandler->createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close);
        using namespace luabridge;
        getGlobalNamespace(L)
            .beginClass<UpdateComponent>("UpdateComponent")
            .addFunction("remove", &UpdateComponent::remove)
            .endClass()
            .beginClass<GraphicsComponent>("GraphicsComponent")
            .addProperty("alpha", &GraphicsComponent::getAlpha, &GraphicsComponent::setAlpha)
            .endClass()
            .beginClass<GameStateSplash>("GameStateSplash")
            .addData("wait", &GameStateSplash::m_wait)
            .addData("state", &GameStateSplash::m_animationState)
            .endClass();
        m_scriptHandler->loadEntities(m_entities, L, "assets/data/gameStateSplash.lua");
        m_isRunning = true;
        m_wait = 0;
        m_animationState = 0;
    }

    bool GameStateSplash::update() {
        m_audioHandler->update();
        m_updateHandler->update();
        switch(m_guiHandler->update()) {
            default:
                break;
        }
        if(m_animationState == 3) {
            m_windowHandler->getWindow().close();
            m_isRunning = false;
        }
        return m_isRunning;
    }

    void GameStateSplash::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color::Black);
        m_graphicsHandler->draw(dt);
        m_windowHandler->getWindow().display();
    }

    void GameStateSplash::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            m_guiHandler->handleInput(windowEvent);
            if(windowEvent.type == sf::Event::Closed) {
                m_windowHandler->getWindow().close();
                m_isRunning = false;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_windowHandler->getWindow().close();
                        m_isRunning = false;
                        return;
                    case sf::Keyboard::F5:
                        m_scriptHandler->loadEntities(m_entities, L, "assets/data/gameStateSplash.lua");
                        LogHandler::log(LogHandler::INF, "Finished reloading script", typeid(*this).name());
                        break;
                    case sf::Keyboard::Pause:
                        LogHandler::log(LogHandler::INF, "Breakpoint", typeid(*this).name());
                        break;
                }
            }
        }
    }
}