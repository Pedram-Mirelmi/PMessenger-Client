#pragma once
#include <QObject>
#include <QByteArray>
#include <QMutex>

//class TSQByteArray
//{
//public:
//    QMutex m_lock;
//    QByteArray m_array;


//    inline constexpr TSQByteArray() noexcept
//        : m_array()
//    {}
//    TSQByteArray(const char * chr, qsizetype size = -1)
//        : m_array(chr, size)
//    {}
//    TSQByteArray(qsizetype size, char c)
//        : m_array(size, c)
//    {}
//    TSQByteArray(qsizetype size, Qt::Initialization option)
//        : m_array(size, option)
//    {};

//    inline TSQByteArray(const QByteArray &exp) = delete;

//    TSQByteArray &operator=(const QByteArray &other) = delete;

//    TSQByteArray &operator=(const char *str) = delete;

//    inline TSQByteArray(QByteArray && other) = delete;

//    inline ~TSQByteArray();

//    void append(char c)
//    {
//        m_lock.lock();
//        this->m_array.append(c);
//        m_lock.unlock();
//    };
//    void append(qsizetype count, char c)
//    {
//        m_lock.lock();
//        this->m_array.append(count, c);
//        m_lock.unlock();
//    }
//    void append(const char *s)
//    {
//        m_lock.lock();
//        this->m_array.append(QByteArrayView(s, qsizetype(qstrlen(s))));
//        m_lock.unlock();
//    }
//    void append(const char *s, qsizetype len)
//    {
//        m_lock.lock();
//        this->m_array.append(QByteArrayView(s, len));
//        m_lock.unlock();
//    }
//    void append(const QByteArray &a)
//    {
//        m_lock.lock();
//        this->m_array.append(a);
//        m_lock.unlock();
//    }
//    void append(QByteArrayView a)
//    {
//        m_lock.lock();
//        this->m_array.insert(this->m_array.size(), a);
//        m_lock.unlock();
//    }

//};
