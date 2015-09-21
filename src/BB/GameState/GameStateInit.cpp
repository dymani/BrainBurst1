#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game) {
        m_windowHandler.createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close, sf::ContextSettings());
        m_isRunning = true;
    }

    bool GameStateInit::update() {
        return m_isRunning;
    }

    void GameStateInit::draw(const double dt) {
        m_windowHandler.getWindow().clear({10, 10, 10, 255});
        m_windowHandler.getWindow().display();
    }

    void GameStateInit::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler.getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed
                || (windowEvent.type == sf::Event::KeyPressed
                    && windowEvent.key.code == sf::Keyboard::Escape)) {
                m_windowHandler.getWindow().close();
                m_isRunning = false;
            }
        }
    }
}