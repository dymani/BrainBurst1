#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/Component/PlayerComponent.h"

namespace bb {
    GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler, luabridge::lua_State* L): IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = windowHandler;
        m_graphicsHandler = new GraphicsHandler(*m_windowHandler);
        this->L = L;
        m_state = RUNNING;
        m_field = new Field(m_resourceHandler, m_graphicsHandler, L, "test",
            m_windowHandler->getWindow().getSize().y, 1);
        for(auto& entity : m_field->getEntities()) {
            if(entity->getComponent<PlayerComponent>()) {
                m_player = entity;
                break;
            }
        }
        m_windowHandler->getWindow().setKeyRepeatEnabled(true);
    }

    void GameStateGame::handleInput() {
        if(m_player && m_player->getComponent<PlayerComponent>()) {
            m_player->getComponent<PlayerComponent>()->handleInput();
        }
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_state = TITLE;
                        break;
                }
            }
        }
    }

    bool GameStateGame::update() {
        m_field->update();
        switch(m_state) {
            case RUNNING:
                break;
            case TITLE:
                m_game.changeState(new GameStateTitle(m_game, m_resourceHandler, m_windowHandler, L));
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
        }
        return true;
    }

    void GameStateGame::draw(const double dt) {
        m_windowHandler->getWindow().clear();
        m_field->draw(m_windowHandler->getWindow());
        m_graphicsHandler->display(dt);
        m_windowHandler->getWindow().display();
    }
}