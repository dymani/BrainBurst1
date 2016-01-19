#include "BB/GameState/GameStateTitle.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    GameStateTitle::GameStateTitle(Game& game, ResourceHandler* resourceHandler,
        WindowHandler* windowHandler) : IGameState(game),
        m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
        m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)) {
        L = luabridge::luaL_newstate();
        luaL_openlibs(L);
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
                m_game.changeState(new GameStateGame(m_game, m_resourceHandler.release(),
                    m_windowHandler.release()));
                break;
        }
        return true;
    }

    void GameStateTitle::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color(0, 255, 0, 255));
        m_windowHandler->getWindow().display();
    }
}