#pragma once

#include "..\GameObject.h"

typedef enum EnemyType_t
{
    Slim,
    FLy
} EnemyType;

class CEnemy : public IGameObject
{
    protected:
        EnemyType_t m_EnemyType;
        bool m_IsAlive;

        Vec2f m_Velocity, m_Position;
        Rect2d m_Rect;

    public:
        CEnemy()
        {
            m_EnemyType = Slim;
            m_IsAlive = true;
        }

        CEnemy(EnemyType_t EnemieType)
        {
            m_EnemyType = EnemieType;
            m_IsAlive = true;
        }

        inline bool IsAlive()
        {
            return m_IsAlive;
        }

        inline void SetEnemies(EnemyType_t EnemieType)
        {
            m_EnemyType = EnemieType;
        }

        inline EnemyType_t GetEnemieType()
        {
            return m_EnemyType;
        }
};