#pragma once

/*
    Project includes
*/
#include "Singleton.h"

/*
    Libs include
*/

/*
    STDLib/OS includes
*/
#include <map>
#include <string>
#include <typeinfo>

class CResourceManager : public CSingleton<CResourceManager>
{
    private:
        typedef std::map<std::string, void *> TBranch;
        typedef std::map<size_t, TBranch> TRepository;

    private:
        TRepository m_Repository;

    public:
        template <class T> void Register()
        {
            m_Repository[typeid(T).hash_code()] = TBranch();
        }

        template <class T> void Add(const std::string &id, T *data)
        {
            TRepository::iterator itr = m_Repository.find(typeid(T).hash_code());
            if (itr == m_Repository.end()) return;

            TBranch::iterator itb = itr->second.find(id);
            if (itb != itr->second.end()) return;

            itr->second[id] = data;
        }

        template <class T> T * Get(const std::string &id)
        {
            TRepository::iterator itr = m_Repository.find(typeid(T).hash_code());
            if (itr == m_Repository.end()) return nullptr;

            TBranch::iterator itb = itr->second.find(id);
            if (itb == itr->second.end()) return nullptr;

            return (T *)itb->second;
        }
};
