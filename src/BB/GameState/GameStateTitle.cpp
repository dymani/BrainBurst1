#include "BB/GameState/GameStateTitle.h"
#include "BB/Game.h"
namespace bb {
    GameStateTitle::GameStateTitle(Game & game, ResourceHandler * resourceHandler,
        WindowHandler* windowHandler, luabridge::lua_State * L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = windowHandler;
        this->L = L;
        m_state = RUNNING;
    }

    bool GameStateTitle::update() {
        if(m_state == QUIT) {
            m_windowHandler->getWindow().close();
        } else if(m_state == GAME) {

        }
        return (m_state != QUIT);
    }

    void GameStateTitle::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color(0, 255, 0, 255));
        m_windowHandler->getWindow().display();
    }

    void GameStateTitle::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_windowHandler->getWindow().close();
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Return:
                        m_state = GAME;
                        break;;
                    case sf::Keyboard::Escape:
                        m_windowHandler->getWindow().close();
                        m_state = QUIT;
                        return;
                }
            }
        }
    }
}