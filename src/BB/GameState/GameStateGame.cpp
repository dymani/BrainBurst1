#include "BB/GameState/GameStateGame.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"
#include "BB/Component/PlayerComponent.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    GameStateGame::GameStateGame(Game& game, ResourceHandler* resourceHandler, WindowHandler* windowHandler, luabridge::lua_State* L) : IGameState(game) {
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
                    case sf::Keyboard::LControl:
                        LogHandler::log(LogHandler::INF, std::to_string(m_world->getEntity(0)->getCoord().x)
                            + " " + std::to_string(m_world->getEntity(0)->getCoord().y), typeid(*this).name());
                        break;
                    case sf::Keyboard::W:
                        m_world->getEntity(0)->getComponent<MovementComponent>()->setVelocityY(50);
                        break;
                    case sf::Keyboard::Q:
                        m_world->getEntity(0)->setCoord(m_world->getEntity(0)->getCoord()
                            + sf::Vector2f(-5, 0));
                        break;
                    case sf::Keyboard::E:
                        m_world->getEntity(0)->setCoord(m_world->getEntity(0)->getCoord()
                            + sf::Vector2f(5, 0));
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
        sf::Vector2f playerCoord = m_world->getEntity(0)->getCoord();
        if(m_graphicsHandler->getViewCoord().x > playerCoord.x + 3) {
            m_graphicsHandler->setViewCoordX(playerCoord.x + 3);
        } else if(m_graphicsHandler->getViewCoord().x < playerCoord.x - 3) {
            m_graphicsHandler->setViewCoordX(playerCoord.x - 3);
        }
        if(m_graphicsHandler->getViewCoord().y < playerCoord.y - 5) {
            m_graphicsHandler->setViewCoordY(playerCoord.y - 5);
        } else if(m_graphicsHandler->getViewCoord().y > playerCoord.y - 3) {
            m_graphicsHandler->setViewCoordY(playerCoord.y - 3);
        }
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