#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game), m_graphicsHandler(m_windowHandler, m_entities),
        m_resourceHandler(game), m_scriptHandler(m_resourceHandler), m_audioHandler(m_resourceHandler),
        m_guiHandler(m_windowHandler, m_entities), m_initFunc(nullptr) {
        m_windowHandler.createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close,
            sf::ContextSettings());
        m_isRunning = true;
        using namespace luabridge;
        L = luaL_newstate();
        luaL_openlibs(L);
        m_resourceHandler.load(L);
        m_scriptHandler.loadEntities(m_entities, L, "assets/data/gameStateInit.lua");
    }

    GameStateInit::~GameStateInit() {
        delete m_initFunc.get();
    }

    bool GameStateInit::update() {
        m_audioHandler.update();
        switch(m_guiHandler.update()) {
            case -1:
                break;
            case 0:
                m_audioHandler.play("test2");
                std::cout << "Pressed 0\n";
                break;
            case 1:
                m_audioHandler.play("test3");
                std::cout << "Pressed 1\n";
                break;
            case 2:
                std::cout << "Pressed 2\n";
                m_windowHandler.getWindow().close();
                m_isRunning = false;
                break;
        }
        return m_isRunning;
    }

    void GameStateInit::draw(const double dt) {
        m_windowHandler.getWindow().clear({0, 0, 0, 255});
        m_graphicsHandler.draw(dt);
        m_windowHandler.getWindow().display();
    }

    void GameStateInit::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler.getWindow().pollEvent(windowEvent)) {
            m_guiHandler.handleInput(windowEvent);
            if(windowEvent.type == sf::Event::Closed) {
                m_windowHandler.getWindow().close();
                m_isRunning = false;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_windowHandler.getWindow().close();
                        m_isRunning = false;
                        return;
                    case sf::Keyboard::F5:
                        m_scriptHandler.loadEntities(m_entities, L, "assets/data/gameStateInit.lua");
                        std::cout << "Script gameStateInit.lua reloaded.\n";
                        break;
                    case sf::Keyboard::Num1:
                        id = m_audioHandler.play("test1");
                        break;
                    case sf::Keyboard::Num2:
                        m_audioHandler.play("test2");
                        break;
                    case sf::Keyboard::Num3:
                        m_audioHandler.play("test3");
                        break;
                    case sf::Keyboard::Space:
                        m_audioHandler.togglePause(id);
                        break;
                }
            }
        }
    }
}