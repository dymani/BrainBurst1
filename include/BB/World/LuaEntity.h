#ifndef LUA_ENTITY_H
#define LUA_ENTITY_H

#include <string>

namespace bb {
    class GameStateGame;

    class LuaEntity {
    public:
        LuaEntity(GameStateGame& game, int entity);
        void hsSetDamage(int damage);
        int hsGetHealth() const;
        void gsSetAnimation(std::string name);
        void gsSetAnimationFrame(int frame);
        int csGetState() const;
        bool csIsFacingLeft() const;
        bool csIsMovingLeft() const;
        void psSetVelocityX(float x);
        void psSetVelocityY(float y);
        float psGetVelocityX() const;
        float psGetVelocityY() const;
        void psSetHitbox(float x, float y, float width, float height);
        void setCoordX(float x);
        void setCoordY(float y);
        float getCoordX() const;
        float getCoordY() const;
        void print(std::string text);
    private:
        GameStateGame& m_game;
        const int m_entity;
    };
}

#endif