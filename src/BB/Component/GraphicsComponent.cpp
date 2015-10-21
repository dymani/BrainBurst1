#include "BB/Component/GraphicsComponent.h"
#include "BB/World/Entity.h"

namespace bb {
    GraphicsComponent::GraphicsComponent(Entity& entity): m_entity(entity) {
    }

    IComponent* GraphicsComponent::copy() {
        GraphicsComponent* gc = new GraphicsComponent(m_entity);
        for(auto& drawable : m_drawables) {
            std::cout << drawable.first.name() << std::endl;
            if(drawable.first.name() == "class sf::Sprite") {
                gc->addDrawable(std::type_index(typeid(sf::Sprite())),
                    new sf::Sprite(*dynamic_cast<sf::Sprite*>(drawable.second.second)), drawable.second.first);
            }
        }
        gc->m_z = m_z;
        return gc;
    }

    void GraphicsComponent::addDrawable(std::type_index type, sf::Drawable* drawable, std::string name) {
        auto pair = std::make_pair(name, drawable);
        m_drawables[type] = pair;
    }

    void GraphicsComponent::setZ(float z) {
        m_z = z;
    }

    void GraphicsComponent::setSize(sf::Vector2i size) {
        m_size = size;
    }

    std::map<std::type_index, std::pair<std::string, sf::Drawable*>>& GraphicsComponent::getDrawables() {
        return m_drawables;
    }

    float GraphicsComponent::getZ() {
        return m_z;
    }

    sf::Vector2i GraphicsComponent::getSize() {
        return m_size;
    }
}