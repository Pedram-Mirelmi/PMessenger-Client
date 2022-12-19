#pragma once

#include <asio.hpp>
#include <string>
#include "./IService.h"
#include "./io/BasicNetMessage.h"
#include "./io/INetMessageBodyDeserializer.h"

using std::shared_ptr;
using std::make_shared;


template<typename MsgType>
class AbstractNetIOManager : public IService,
                          public std::enable_shared_from_this<AbstractNetIOManager<MsgType>>
{
    using socket = asio::ip::tcp::socket;
    using end_point = asio::ip::tcp::endpoint;
    using string = std::string;
protected:
    asio::io_context m_ioContext;
    socket m_socket;
    end_point m_endPoint;

    bool m_isConnected = false;

    std::vector<char> m_headerInBuffer, m_bodyInBuffer;
    NetMessageHeader<MsgType> m_tempHeader;

    shared_ptr<INetMessageBodyDeserializer<MsgType>> m_bodyDeserializer;

public:
    AbstractNetIOManager(const string& ip, uint16_t port)
        : IService(), m_socket(m_ioContext)
    {
        m_endPoint.address(asio::ip::make_address(ip));
        m_endPoint.port(port);
        m_headerInBuffer.resize(NetMessageHeader<MsgType>::getHeaderSize());
    }
    ~AbstractNetIOManager()
    {
        this->stop();
    }
protected:
    virtual void onAsyncConnected(std::error_code ec, asio::ip::tcp::endpoint endPoint)
    {
        if(!ec)
        {
            m_isConnected = true;
            asio::async_read(m_socket,
                             asio::buffer(m_headerInBuffer.data(), NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );



        }
        else
        {

        }
    }

    virtual void onAsyncReadHeader(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            deserializeHeader();
            m_bodyInBuffer.resize(m_tempHeader.getBodySize());
            asio::async_read(m_socket,
                             asio::buffer(m_bodyInBuffer.data(), m_tempHeader.getBodySize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadBody,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
        }
        else
        {

        }
    }

    virtual void onAsyncReadBody(std::error_code ec, std::size_t length)
    {
        if(!ec)
        {
            onNewMessageReadCompletely();
            asio::async_read(m_socket,
                             asio::buffer(m_headerInBuffer, NetMessageHeader<MsgType>::getHeaderSize()),
                             std::bind(&AbstractNetIOManager::onAsyncReadHeader,
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2)
                             );
        }
    }

    virtual void onAsyncWrite(std::error_code ec, std::size_t length, const char* msgBuffer, size_t msgSize)
    {
        if(!ec)
        {
            delete [] msgBuffer;
        }
        else
        {

        }
    }

    void deserializeHeader()
    {
        m_tempHeader.deserialize(m_headerInBuffer.data());
    }

    virtual void onNewMessageReadCompletely() = 0;
public:
    virtual void writeMessage(shared_ptr<BasicNetMessage<MsgType>> msg)
    {
        uint32_t msgSize = msg->getHeader().getBodySize() + msg->getHeader().calculateNeededSizeForThis();
        char* msgBuffer = new char[msgSize];
        msg->serialize(msgBuffer);
        asio::async_write(m_socket,
                          asio::buffer(msgBuffer, msgSize),
                          std::bind(&AbstractNetIOManager::onAsyncWrite,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          msgBuffer,
                          msgSize)
                          );
    }

    // IService interface
public:
    virtual void start() override
    {
//        asio::async_connect(m_socket,
//                            m_endPoint,
//                            std::bind(&AbstractNetIOManager::onAsyncConnected,
//                                      this,
//                                      std::placeholders::_1,
//                                      std::placeholders::_2));
    }
    virtual void stop() override
    {

    }

    void setBodyDeserializer(const shared_ptr<INetMessageBodyDeserializer<MsgType> > &newBodyDeserializer)
    {
        m_bodyDeserializer = newBodyDeserializer;
    }
};




