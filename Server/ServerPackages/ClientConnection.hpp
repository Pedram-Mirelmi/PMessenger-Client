#pragma once
#include <vector>
#include <thread>
// #include <stdlib.h>
#include <memory>
#include <mutex>
#include <unordered_set>

#include <jsoncpp/json/json.h>
#include "./NetworkHandler.hpp"
#include "ServerRequestHandler.hpp"

#define INVALID_ID -1

class ClientConnection
{
typedef std::unordered_map<id_T, std::shared_ptr<ClientConnection>> conn_set;
private:
	// sockaddr_in hint; 
    NetworkHandler networkHandler;
    id_T user_id = INVALID_ID;
    conn_set& other_connections;
    JsonObj current_request;
    JsonObj current_response;
    std::chrono::steady_clock::time_point last_req_time;
    ServerRequestHandler rh;
public:
    static std::chrono::steady_clock::duration maxSuspension;
   
    ClientConnection(const int& socket, conn_set& others)
        : networkHandler(socket), other_connections(others), rh(user_id)
    {};

    id_T handleLoginOrRegister()
    { 
        using namespace KeyWords;
        try
        {   
            while (true)
            {
                current_response.clear();
                current_request.clear();
                this->networkHandler.receiveMessage(current_request);
                if (current_request[NET_MESSAGE_TYPE] == CLOSE_CONNECTION)
                {
                    std::cout << "connection closed properly by client" << std::endl;
                    return INVALID_ID;
                }
                std::cout << (current_request[NET_MESSAGE_TYPE].asString() == REGISTER) << std::endl;
                this->rh.handle(current_request, current_response);
                if ((current_request[NET_MESSAGE_TYPE].asString() == REGISTER
                     || current_request[NET_MESSAGE_TYPE].asString() == LOGIN) 
                     && current_response[SUCCESSFUL].asBool())
                {
                    std::cout << current_response << std::endl;
                    this->user_id = std::stoi(current_response[USER_INFO][USER_ID].asString());
                    this->networkHandler.sendMessage(current_response);
                    break;
                }
                this->networkHandler.sendMessage(current_response);
            }
            current_request.clear();
            current_response.clear();
            std::thread(&ClientConnection::controlOnlineTime, this).detach();
            return this->user_id;
        }
        catch (std::exception& e)
        {
            
            std::cout << e.what() << std::endl;
            return INVALID_ID;
        }
    }

    void talk()
    {
        using namespace KeyWords;
        try
        {
            while (true)
            {
                current_response.clear();
                current_request.clear();
                this->networkHandler.receiveMessage(current_request);
                if (current_request[NET_MESSAGE_TYPE].asCString() == CLOSE_CONNECTION)
                {
                    std::cout << "connection closed properly by client" << std::endl;
                    return;
                }
                std::thread(&ClientConnection::controlOnlineTime, this).detach();
                this->rh.handle(current_request, current_response);
                this->networkHandler.sendMessage(current_response);
                std::thread(&ClientConnection::checkForSpecialOperations, this, current_request, current_response).detach();
                current_request.clear();
                current_response.clear();
            }
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

private:
    void controlOnlineTime()
    {
        if (std::chrono::steady_clock::now() - this->last_req_time > ClientConnection::maxSuspension)
            this->rh.setMeOnline(this->user_id);
        this->last_req_time = std::chrono::steady_clock::now();
        std::this_thread::sleep_for(this->maxSuspension);
        if (std::chrono::steady_clock::now() - this->last_req_time > ClientConnection::maxSuspension)
            this->rh.setMeOffline(this->user_id);
    }

    void checkForSpecialOperations(msg_t request, msg_t response)
    {
        using namespace KeyWords;
        if (request[NET_MESSAGE_TYPE] == SEND_NEW_MESSAGE
            && response[SUCCESSFUL].asBool() == true)
        {
            this->announceNewMessageToOthers(response[MESSAGE_INFO]);
        }
    }

    void announceNewMessageToOthers(msg_t& message_info)
    {
        using namespace KeyWords;
        std::unordered_set<std::string> audiences;
        this->rh.getParticipants(message_info[ENV_ID].asInt64(), audiences);
        std::vector<std::thread> threads;
        threads.reserve(audiences.size());
        for (const std::string& id_str : audiences)
            threads.emplace_back([&]() 
                {
                    auto audience = other_connections[std::stoi(id_str)];
                    if (audience != NULL)
                        audience->announceNewMessageToMe(current_request);
                }
            );
        for (auto& thread : threads)
            thread.join();

        return;
    }

public:
    void announceNewMessageToMe(msg_t message_info)
    {
        this->networkHandler.sendMessage(message_info);
    }
};


