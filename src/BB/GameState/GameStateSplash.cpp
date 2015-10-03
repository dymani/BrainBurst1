#include "BB/GameState/GameStateSplash.h"
#include "BB/Game.h"

namespace bb {
    GameStateSplash::GameStateSplash(Game& game, ResourceHandler* resourceHandler,
        luabridge::lua_State* L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_audioHandler = new AudioHandler(*m_resourceHandler);
        m_windowHandler = new WindowHandler();
        m_graphicsHandler = new GraphicsHandler(*m_windowHandler, m_entities);
        m_guiHandler = new GuiHandler(*m_windowHandler, m_entities);
        m_scriptHandler = new ScriptHandler(*m_resourceHandler);
        this->L = L;
        m_windowHandler->createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close);
        m_scriptHandler->loadEntities(m_entities, L, "assets/data/gameStateSplash.lua");
        m_isRunning = true;
    }

    bool GameStateSplash::update() {
        m_audioHandler->update();
        switch(m_guiHandler->update()) {
            default:
                break;
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