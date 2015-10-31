#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/Component/PlayerComponent.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler, luabridge::lua_State* L): IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = windowHandler;
        m_graphicsHandler = new GraphicsHandler(*this);
        this->L = L;
        m_state = RUNNING;
        m_world = new World(*this, "test", L);
        m_world->createField();
        m_windowHandler->getWindow().setKeyRepeatEnabled(true);
    }

    void GameStateGame::handleInput() {
        m_world->handleInput();
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            m_world->handleInput(windowEvent);
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
        m_world->update();
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
        m_world->draw();
        m_graphicsHandler->display(dt);
        m_windowHandler->getWindow().display();
    }

    World* GameStateGame::getWorld() {
        return m_world;
    }

    ResourceHandler* GameStateGame::getResourceHandler() {
        return m_resourceHandler;
    }

    GraphicsHandler* GameStateGame::getGraphicsHandler() {
        return m_graphicsHandler;
    }

    WindowHandler* GameStateGame::getWindowHandler() {
        return m_windowHandler;
    }

    PhysicsHandler* GameStateGame::getPhysicsHandler() {
        return m_physicsHandler;
    }
}