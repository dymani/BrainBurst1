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
        Entity* player = new Entity(0);
        player->setCoord({0, 0});
        MovementComponent* mc = new MovementComponent(*this, 0);
        mc->setVelocity({0, 0});
        player->addComponent(std::type_index(typeid(*mc)), mc);
        PlayerComponent* pc = new PlayerComponent(*this, 0);
        player->addComponent(std::type_index(typeid(*pc)), pc);
        GraphicsComponent* gc = new GraphicsComponent(*this, 0);
        gc->addDrawable("default", new sf::Sprite(m_resourceHandler->getTexture("NULL"), {0, 0, 64, 64}), 10);
        player->addComponent(std::type_index(typeid(*gc)), gc);
        m_entities.clear();
        m_entities.push_back(player);
        m_field = new Field(*this, L, "test", m_windowHandler->getWindow().getSize().y, 1);
        m_windowHandler->getWindow().setKeyRepeatEnabled(true);
    }

    void GameStateGame::handleInput() {
        m_entities[0]->getComponent<PlayerComponent>()->handleInput();
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
        m_movementHandler->update(m_entities);
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
        m_field->draw();
        for(unsigned int i = 0; i < m_entities.size(); i++) {
            m_graphicsHandler->draw(int(i));
        }
        m_graphicsHandler->display(dt);
        m_windowHandler->getWindow().display();
    }

    Entity* GameStateGame::getEntity(int id) {
        if(m_entities[id])
            return m_entities[id];
        return nullptr;
    }

    std::vector<Entity*>& GameStateGame::getEntities() {
        return m_entities;
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
    MovementHandler * GameStateGame::getMovementHandler() {
        return m_movementHandler;
    }
}