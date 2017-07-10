#pragma once

#include "..\GameObject.h"

typedef enum EnemyType_t
{
    Slim,
    FLy
} EnemyType;

class CEnemy : public IGameObject
{
    private:
        EnemyType_t m_EnemyType;

    public:
        CEnemy()
        {
            m_EnemyType = Slim;
        }

        CEnemy(EnemyType_t EnemieType)
        {
            m_EnemyType = EnemieType;
        }

        void SetEnemies(EnemyType_t EnemieType)
        {
            m_EnemyType = EnemieType;
        }

        EnemyType_t GetEnemieType()
        {
            return m_EnemyType;
        }
};