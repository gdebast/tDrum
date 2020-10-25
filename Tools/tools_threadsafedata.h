#pragma once

#include <shared_mutex>

namespace Tools
{
    template<class StoredData>
    class ThreadSafeDataAccessUnique
    {

        public:
            ThreadSafeDataAccessUnique(StoredData &storedData, std::shared_mutex &mutex) :
                m_storedDate(storedData),
                m_shared_mutex(mutex)
            {
                m_shared_mutex.lock();
            }

            ~ThreadSafeDataAccessUnique()
            {
                m_shared_mutex.unlock();
            }

            StoredData  &get()
            {
                return m_storedDate;
            }

            StoredData  *operator->()
            {
                return &m_storedDate;
            }

            void assign(StoredData& newStoredData)
            {
                m_storedDate = std::move(newStoredData);
            }


        private:
            StoredData        &m_storedDate;
            std::shared_mutex &m_shared_mutex;


    };

    template<class StoredData>
    class ThreadSafeDataAccessShared
    {

        public:
            ThreadSafeDataAccessShared(const StoredData &storedData, std::shared_mutex &mutex) :
                m_storedDate(storedData),
                m_shared_mutex(mutex)
            {
                m_shared_mutex.lock_shared();
            }

            ~ThreadSafeDataAccessShared()
            {
                m_shared_mutex.unlock_shared();
            }

            const StoredData  &get()
            {
                return m_storedDate;
            }

            const StoredData  *operator->()
            {
                return &m_storedDate;
            }


        private:
            const StoredData  &m_storedDate;
            std::shared_mutex &m_shared_mutex;


    };

}

namespace Tools
{
    template<class StoredData>
    class ThreadSafeData
    {
        public:
            ThreadSafeData(){}
            ThreadSafeData(StoredData &&storedData) : m_storedDate(storedData){}
            ThreadSafeData(const ThreadSafeData&) = delete;
            ThreadSafeData(ThreadSafeData&&) = delete;
            ThreadSafeData& operator=(const ThreadSafeData&) = delete;
            ThreadSafeData& operator=(ThreadSafeData&&) = delete;

            ThreadSafeDataAccessUnique<StoredData> lock()
            {
                return ThreadSafeDataAccessUnique(m_storedDate,m_shared_mutex);
            }
            ThreadSafeDataAccessShared<StoredData> share() const
            {
                return ThreadSafeDataAccessShared(m_storedDate,m_shared_mutex);
            }

        private:
            StoredData                m_storedDate;
            mutable std::shared_mutex m_shared_mutex;
    };

}

