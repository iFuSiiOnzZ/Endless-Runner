#pragma once

template<typename T> class CSingleton
{
    private:
        CSingleton(CSingleton const&) {};
        CSingleton& operator=(CSingleton const&) {};

    protected:
        static T* m_instance;

        CSingleton() { m_instance = static_cast <T*> (this); };
        ~CSingleton() {  };

    public:
        static T* GetInstance()
        {
            if (nullptr == m_instance) CSingleton<T>::m_instance = new T();
            return m_instance;
        }

        static void Destroy()
        {
            delete CSingleton<T>::m_instance;
            CSingleton<T>::m_instance = 0;
        }
};

template<typename T> T* CSingleton<T>::m_instance = 0;