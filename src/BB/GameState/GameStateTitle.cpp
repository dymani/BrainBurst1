#include "BB/GameState/GameStateTitle.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    GameStateTitle::GameStateTitle(Game & game, ResourceHandler * resourceHandler,
        WindowHandler* windowHandler, luabridge::lua_State * L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = windowHandler;
        this->L = L;
        m_state = RUNNING;
    }

    void GameStateTitle::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Return:
                    case sf::Keyboard::Space:
                        m_state = GAME;
                        break;
                    case sf::Keyboard::Escape:
                        m_state = QUIT;
                        return;
                }
            }
        }
    }

    bool GameStateTitle::update() {
        switch(m_state) {
            case RUNNING:
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
            case GAME:
                m_game.changeState(new GameStateGame(m_game, m_resourceHandler, m_windowHandler, L));
                break;
        }
        return true;
    }

    void GameStateTitle::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color(0, 255, 0, 255));
        m_windowHandler->getWindow().display();
    }
}