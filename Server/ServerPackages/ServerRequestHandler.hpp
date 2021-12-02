#pragma once
#include <fmt/format.h>
#include "../../Commons/KeyWords.h"
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
    {

    }

    void handle(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        const std::string& req_type = request[NET_MESSAGE_TYPE].asString();
        std::cout << req_type;
        if (req_type == REGISTER)
        {
            this->handleRegister(request, response);
            response[NET_MESSAGE_TYPE] = REGISTER_RESULT;
            return;
        }
        if (req_type == LOGIN)
        {
            this->handleLogin(request, response);
            response[NET_MESSAGE_TYPE] = LOGIN_RESULT;
            return;
        }
        if (req_type == USER_SEARCH)
        {
            this->handleUserSearch(request, response);
            response[NET_MESSAGE_TYPE] = USER_SEARCH_RESULT;
            return;
        }
        if (req_type == SEND_NEW_MESSAGE)
        {
            this->handleNewMessage(request, response);
            response[NET_MESSAGE_TYPE] = MESSAGE_SENT_CONFIRMATION;
            return;
        }
        if (req_type == CREATE_NEW_CHAT)
        {
            this->handleCreateNewChat(request, response);
            response[NET_MESSAGE_TYPE] = CHAT_CREATION_CONFIRMATION;
            return;
        }
        if (req_type == FETCH_ALL)
        {
            this->handleFetchAll(request, response);
            response[NET_MESSAGE_TYPE] = DATA;
            response[NET_MESSAGE_TYPE] = FETCH_ALL_RESULT;
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
        auto query = fmt::format("INSERT INTO users(username, password) VALUES ('{}', '{}');",
                                    request[USERNAME].asString(), request[PASSWORD].asString());
       
        bool successful;
        response[DETAILS] = this->db.executeQry(query, successful);
        int user_id = getLastInsertId();
        if (successful)
        {
            auto Qry = fmt::format("SELECT * FROM users WHERE user_id = {};", "LAST_INSERT_ID()");
            this->db.singleSELECT(Qry, response[USER_INFO]);
            response[SUCCESFUL] = TRUEE;
            return;
        }
        response[SUCCESFUL] = FALSEE;
    }

    void handleLogin(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM {} WHERE {} = '{}';", USERS, USERNAME, request[USERNAME].asString());
        this->db.SELECT(Qry, response);
        if (response[PASSWORD] == request[PASSWORD])
        {
            response[SUCCESFUL] = TRUEE;
            return;
        }
        response.clear();
        response[SUCCESFUL] = FALSEE;        
        return;
    }

    void handleFetchAll(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto privates_query = fmt::format("SELECT * from private_attendees WHERE user_id = {};", this->m_user_id);
        auto groups_query = fmt::format("SELECT * from groups_attendees WHERE user_id = {};", this->m_user_id);
        auto channels_query = fmt::format("SELECT * from channel_attendees WHERE user_id = {};", this->m_user_id);
        JsonObj privates, groups, channels;
        std::cout << this->db.SELECT(privates_query, privates);
        std::cout << this->db.SELECT(groups_query, groups);
        std::cout << this->db.SELECT(channels_query, channels);
        response[PRIVATE_CHATS] = privates;
        response[GROUP_CHATS] = groups;
        response[CHANNELS] = channels;
    }


    void handleUserSearch(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM {} WHERE username LIKE {} OR name LIKE {}", USERS, request[SEARCH_PHRASE].asString(), request[SEARCH_PHRASE].asString());
        Json::Value search_result;
        this->db.SELECT(Qry, search_result);
        response[RESULT] = search_result;
        response[SUCCESFUL] = true;
    }

    void handleCreateNewChat(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto create_env_Qry = fmt::format("INSERT INTO {}() VALUES ();",
                        MESSAGES, CHAT_ENVS);
        JsonObj temp;
        this->db.singleSELECT("SELECT LAST_INSERT_ID();", temp);
        auto env_id = temp[0].asUInt64();
        auto add_first_user = fmt::format("INSERT INTO {}({}, {}) VALUES ({}, {});",
                                        CHAT_ATTENDS, USER_ID, ENV_ID, this->m_user_id, env_id);
        auto add_second_user = fmt::format("INSERT INTO {}({}, {}) VALUES ({}, {});",
                                        CHAT_ATTENDS, USER_ID, ENV_ID, request[CONTACT_ID].asUInt64(), env_id);

        bool successful1, successful2;
        
        this->db.executeQry(create_env_Qry, successful1);
        this->db.executeQry(add_first_user, successful1);
        this->db.executeQry(add_second_user, successful2);
        if (successful1 && successful2)
        {
            response[CONTACT_ID] = request[CONTACT_ID];
            response[ENV_ID] = env_id;
            response[SUCCESFUL] = TRUEE;
            return;
        }
        response[SUCCESFUL] = FALSEE;
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
        auto insert_message_Qry = fmt::format("INSERT INTO messages({}, {}) VALUES ('{}', '{}');",
                                OWNER_ID, ENV_ID, request[ENV_ID].asInt(), this->m_user_id);
        int message_id;
        bool successful;
        std:: cout << this->db.executeQry(insert_message_Qry, successful);
        if(successful)
            message_id = this->getLastInsertId();
        else
            return;
        auto insert_text_message_Qry = fmt::format
                                    ("INSERT INTO text_messages(message_id, message_text) VALUES({}, {});",
                                        message_id, toRaw(request[MESSAGE_TEXT].asString()));
        std::cout << this->db.executeQry(insert_message_Qry, successful);
        if (successful)
        {
            auto Qry = fmt::format("SELECT * FROM messages_view WHERE message_id = {};", message_id);
            this->db.singleSELECT(Qry, response[MESSAGE_INFO]);
            response[SUCCESFUL] = TRUEE;
            return;
        }
        response[SUCCESFUL] = FALSEE;
    }

    void addContact(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        bool successful;
        auto query = fmt::format("INSERT INTO {}({}, {}) VALUES ('{}', '{}');",
                            CONTACTS, USER_ID, CONTACT_ID, request[USER_ID].asString(),
                            request[CONTACT_ID].asString());
        
        response[DETAILS] = this->db.executeQry(query, successful); 
        response[SUCCESFUL] = successful ? TRUEE : FALSEE;
        return;
    }   

    void getContacts(JsonObj& request, JsonObj& response)
    {
        
    }


private:
    int getLastInsertId()
    {
        JsonObj result;
        this->db.singleSELECT("SELECT LAST_INSERT_ID() AS id;", result);
        return result["id"].asInt64();
    }
};

