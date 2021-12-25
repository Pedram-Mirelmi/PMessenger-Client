#pragma once
#include <fmt/format.h>
#include "../../Commons/KeyWords.hpp"
#include "DatabaseConnector.hpp"
#include <unordered_set>
#include "../../Commons/stringTools.hpp"

typedef Json::Value JsonObj;
typedef unsigned long int id_T;
typedef std::unordered_set<std::string> set;
typedef Json::Value JsonObj;

class ServerRequestHandler
{
private:
    id_T& m_user_id;
public:
    DBConnector db;
    ServerRequestHandler(id_T& user_id)
        : m_user_id(user_id)
    {}

    void handle(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        const std::string& net_msg_type = request[NET_MESSAGE_TYPE].asString();
        std::cout << net_msg_type;
        if (net_msg_type == REGISTER)
        {
            this->handleRegister(request, response);
            response[NET_MESSAGE_TYPE] = REGISTER_RESULT;
            return;
        }
        if (net_msg_type == LOGIN)
        {
            this->handleLogin(request, response);
            response[NET_MESSAGE_TYPE] = LOGIN_RESULT;
            return;
        }
        if (net_msg_type == USER_SEARCH)
        {
            this->handleUserSearch(request, response);
            response[NET_MESSAGE_TYPE] = USER_SEARCH_RESULT;
            return;
        }
        if (net_msg_type == SEND_NEW_MESSAGE)
        {
            this->handleNewMessage(request, response);
            response[NET_MESSAGE_TYPE] = DATA;
            response[DATA_TYPE] = MESSAGE_SENT_CONFIRMATION;
            return;
        }
        if (net_msg_type == CREATE_NEW_PRIVATE_CHAT)
        {
            this->handleCreateNewPrivateChat(request, response);
            response[NET_MESSAGE_TYPE] = CHAT_CREATION_CONFIRMATION;
            return;
        }
        if (net_msg_type == FETCH)
        {
            this->handleFetch(request, response);
            response[NET_MESSAGE_TYPE] = DATA;
            response[DATA_TYPE] = FETCH_RESULT;
            return;
        }
    }

    void setMeOffline(const id_t& user_id)
    {

    } 
    
    void setMeOnline(const id_t& user_id)
    {

    }
    
    void getParticipants(const int64_t& env_id_str, set& participants)
    {

    }

    void handleRegister(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto created_user_id = this->db.insertNewUser(request[USERNAME].asCString(), request[PASSWORD].asCString());
        if (created_user_id)
        {
            auto Qry = fmt::format("SELECT * FROM users WHERE user_id = {};", created_user_id);
            this->db.singleSELECT(Qry, response[USER_INFO]);
            response[SUCCESFUL] = true;
            return;
        }
        response[SUCCESFUL] = false;
    }

    void handleLogin(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM {} WHERE {} = '{}';", USERS, USERNAME, request[USERNAME].asString());
        this->db.SELECT(Qry, response);
        if (response[PASSWORD] == request[PASSWORD])
        {
            response[SUCCESFUL] = true;
            return;
        }
        response.clear();
        response[SUCCESFUL] = false;        
        return;
    }

    void handleFetch(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto privates_query = fmt::format("SELECT * from private_chats_view WHERE first_person = {} OR second_person = {};", this->m_user_id, this->m_user_id);
        // auto groups_query = fmt::format("SELECT * from groups_attendees WHERE user_id = {};", this->m_user_id);
        // auto channels_query = fmt::format("SELECT * from channel_attendees WHERE user_id = {};", this->m_user_id);
        JsonObj privates;
                // groups,
                // channels;
        std::cout << this->db.SELECT(privates_query, privates);
        response[PRIVATE_CHATS] = privates;
        // response[GROUP_CHATS] = groups;
        // response[CHANNELS] = channels;
    }

    void handleGetEnvMessages(const JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto text_messages_query = fmt::format("SELECT * from text_messages_view WHERE env_id={}", request[ENV_ID].asUInt64());
        JsonObj text_messages;
        this->db.SELECT(text_messages_query, text_messages);
        response[TEXT_MESSAGES] = text_messages;
    };

    void handleUserSearch(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM users WHERE username LIKE {} OR name LIKE {}", request[SEARCH_PHRASE].asString(), request[SEARCH_PHRASE].asString());
        Json::Value search_result;
        this->db.SELECT(Qry, search_result);
        response[RESULT] = search_result;
        response[SUCCESFUL] = true;
    }

    void handleCreateNewPrivateChat(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto created_env_id = this->db.insertPrivateChatBetween(this->m_user_id, request[USER_ID].asUInt64());
        if (created_env_id)
        {
            auto get_info_query = fmt::format("SELECT * FROM private_chats_view WHERE env_id = {}", created_env_id);
            this->db.singleSELECT(get_info_query, response[ENV_INFO]);
            response[ENV_INFO][ENV_TYPE] = PRIVATE_CHAT;
            response[SUCCESFUL] = true;
            return;
        }
        response[SUCCESFUL] = false;
    }

    void handleNewMessage(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto message_type = request[MESSAGE_TYPE].asString();
        if (message_type == TEXT_MESSAGE)
            handleNewTextMessage(request, response);
    }

    void handleNewTextMessage(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto created_message_id = this->db.insertNewTextMessage(this->m_user_id, request[ENV_ID].asUInt64(), request[MESSAGE_TEXT].asCString());
        if (created_message_id)
        {
            auto get_info_query = fmt::format("SELECT * FROM text_messages_view WHERE message_id = {};", created_message_id);
            this->db.singleSELECT(get_info_query, response[MESSAGE_INFO]);
            response[SUCCESFUL] = true;
            return;
        }
        response[SUCCESFUL] = false;
    }

    void addContact(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        bool successful;
        auto query = fmt::format("INSERT INTO {}({}, {}) VALUES ('{}', '{}');",
                            CONTACTS, USER_ID, CONTACT_ID, request[USER_ID].asString(),
                            request[CONTACT_ID].asString());
        
        response[DETAILS] = this->db.executeQry(query, successful); 
        response[SUCCESFUL] = successful;
        return;
    }   

    void getContacts(JsonObj& request, JsonObj& response)
    {
        
    }

};

