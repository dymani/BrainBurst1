#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game), m_graphicsHandler(m_windowHandler, m_entities),
        m_resourceHandler(game), m_initFunc(nullptr) {
        m_windowHandler.createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close,
            sf::ContextSettings());
        m_isRunning = true;
        using namespace luabridge;
        L = luaL_newstate();
        luaL_openlibs(L);
        m_resourceHandler.load(L);
        getGlobalNamespace(L)
            .beginClass<GameStateInit>("GameStateInit")
            .addFunction("addEntity", &GameStateInit::addEntity)
            .endClass();
        loadScript();
    }

    GameStateInit::~GameStateInit() {
        delete m_initFunc.get();
    }

    bool GameStateInit::update() {
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
                    case sf::Keyboard::Space:
                        loadScript();
                        break;
                }
            }
        }
    }

    void GameStateInit::loadScript() {
        for(auto& entity : m_entities) {
            delete entity;
        }
        m_entities.clear();
        using namespace luabridge;
        if(luaL_dofile(L, "assets/data/gameStateInit.lua") == 0) {
            LuaRef table = getGlobal(L, "gameStateInit");
            if(table.isTable()) {
                if(table["init"].isFunction()) {
                    m_initFunc = std::make_shared<LuaRef>(table["init"]);
                } else {
                    m_initFunc.reset();
                    std::cerr << "Error while loading function \"init\" gameStateInit.lua.\n";
                }
            }
        } else {
            std::cerr << "Error while loading gameStateInit.lua.\n";
        }
        if(m_initFunc) {
            try {
                (*m_initFunc)(this);
            } catch(luabridge::LuaException const& e) {
                std::cerr << "Error while executing init() gameStateInit.lua.\n";
                std::cerr << "  LuaException: " << e.what() << std::endl;
            }
        }
    }

    void GameStateInit::addEntity(std::string textureName, float x, float y, float z) {
        Entity* entity = new Entity();
        GraphicsComponent* gc = new GraphicsComponent();
        gc->setTexture(m_resourceHandler.getTexture(textureName))->setZ(z);
        entity->addComponent(std::type_index(typeid(GraphicsComponent)), gc);
        entity->setCoord({x, y});
        m_entities.push_back(entity);
    }
}