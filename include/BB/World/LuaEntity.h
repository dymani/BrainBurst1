#ifndef LUA_ENTITY_H
#define LUA_ENTITY_H

#include <string>
#include <LuaBridge/LuaBridge.h>

namespace bb {
    class GameStateGame;

    class LuaEntity {
    public:
        static void registerState(luabridge::lua_State* L);
        LuaEntity(GameStateGame& game, int entity);
        void hsSetDamage(int damage);
        int hsGetHealth() const;
        void gsSetAnimation(std::string name);
        void gsSetAnimationFrame(int frame);
        void psSetVelocityX(float x);
        void psSetVelocityY(float y);
        void psSetOnGround(bool onGround);
        bool psGetOnGround() const;
        float psGetVelocityX() const;
        float psGetVelocityY() const;
        void psSetHitbox(float x, float y, float width, float height);
        void setCoordX(float x);
        void setCoordY(float y);
        float getCoordX() const;
        float getCoordY() const;
        int getId() const;
        void print(std::string text);
        void createEntity(std::string name, float x, float y);
    private:
        GameStateGame& m_game;
        const int m_entity;
    };
}

#endif