#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game), m_graphicsHandler(m_windowHandler),
        m_resourceHandler(game) {
        m_windowHandler.createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close, sf::ContextSettings());
        m_isRunning = true;
        m_resourceHandler.load();
        Entity* entity = new Entity();
        GraphicsComponent* gc = new GraphicsComponent();
        gc->setTexture(m_resourceHandler.getTexture("test1"));
        entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
        entity->setZ(0)->setCoord({100, 100});
        m_graphicsHandler.addEntity(entity);

        entity = new Entity();
        gc = new GraphicsComponent();
        gc->setTexture(m_resourceHandler.getTexture("test2"));
        entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
        entity->setZ(2)->setCoord({150, 175});
        m_graphicsHandler.addEntity(entity);

        entity = new Entity();
        gc = new GraphicsComponent();
        gc->setTexture(m_resourceHandler.getTexture("test3"));
        entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
        entity->setZ(1)->setCoord({200, 175});
        m_graphicsHandler.addEntity(entity);
    }

    bool GameStateInit::update() {
        return m_isRunning;
    }

    void GameStateInit::draw(const double dt) {
        m_windowHandler.getWindow().clear({10, 10, 10, 255});
        m_graphicsHandler.draw(dt);
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