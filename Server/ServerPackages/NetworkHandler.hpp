#include <iostream>
#include <string>
#include <mutex>
#include <unordered_map>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <jsoncpp/json/json.h>
#include <cmath>
#include "../../Commons/KeyWords.hpp"


class NetworkHandler 
{
    typedef Json::Value msg_t;
private:
    int sock;
    Json::Reader m_Jparser;
    Json::FastWriter m_Jwriter;
    std::mutex send_lock;
    std::mutex receive_lock;

public:
    NetworkHandler(const int& socket)
        :sock(socket)
    {}

    NetworkHandler(const NetworkHandler&) = delete;
    NetworkHandler& operator=(const NetworkHandler&) = delete;
    
    ~NetworkHandler()
    {
        closeConnection();
    }
    
public:
   
    void receiveMessage(Json::Value& request)
    {
        std::lock_guard<std::mutex> lock(receive_lock);
        int request_length = getHeader();
        int bytes_recv = 0;
        char* buff = new char[request_length + 1];
        while (bytes_recv != request_length)
            bytes_recv += recv(sock, &buff[bytes_recv], request_length - bytes_recv, 0);
        
        buff[request_length] = 0;
        this->m_Jparser.parse(buff, request);
        delete[] buff;
    }

    void sendMessage(const msg_t& message)
    {
        std::lock_guard<std::mutex> lock(send_lock);
        const auto message_string = this->m_Jwriter.write(message);
        std::cout << "sending: " << message_string << std::endl;
        send(this->sock,
                         (std::string(8 - std::ceil(log10((int)(message_string.length()))), ' ')
                                      + std::to_string(message_string.length())).data(),
                         8, MSG_NOSIGNAL);
        if (send(this->sock, message_string.data(), message_string.size(), MSG_NOSIGNAL) == -1)
            throw std::runtime_error("connection closed by client");
    }



private:
    int getHeader()
    {   
        char* req_len_buff = new char[8];
        recv(this->sock, req_len_buff, 8, 0);
        std::cout << req_len_buff << std::endl;
        return std::stoi(req_len_buff);
    };

    void closeConnection()
    {
        try
        {
            msg_t closing_msg;
            closing_msg[KeyWords::NET_MESSAGE_TYPE] = KeyWords::CLOSE_CONNECTION;
            this->sendMessage(closing_msg);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
};

