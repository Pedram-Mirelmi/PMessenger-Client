#pragma once
#include <fmt/format.h>
#include "../../Commons/KeyWords.h"
#include "DatabaseConnector.hpp"
#include <unordered_set>

// typedef Json::Value QryRes;
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
        const std::string& req_type = request[MESSAGE_TYPE].asString();
        if (req_type == REGISTER)
        {
            this->handleRegister(request, response);
            response[MESSAGE_TYPE] = ENTERING;
            return;
        }
        if (req_type == LOGIN)
        {
            this->handleLogin(request, response);
            response[MESSAGE_TYPE] = ENTERING;
            return;
        }
        if (req_type == USER_SEARCH)
        {
            this->handleUserSearch(request, response);
            response[MESSAGE_TYPE] = USER_SEARCH_RESULT;
            return;
        }
        if (req_type == NEW_MESSAGE)
        {
            this->handleNewMessage(request, response);
            response[MESSAGE_TYPE] = MESSAGE_SENT_CONFIRMATION;
            return;
        }
        if (req_type == CREATE_NEW_CHAT)
        {
            this->handleCreateNewChat(request, response);
            response[MESSAGE_TYPE] = CHAT_CREATION_CONFIRMATION;
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
        auto query = fmt::format("INSERT INTO {}({}, {}) VALUES ('{}', '{}');",
                        USERS, USERNAME, PASSWORD, request[USERNAME].asString(), request[PASSWORD].asString());
       
        bool successful;
        response[DETAILS] = this->db.executeQry(query, successful);
        if (successful)
        {
            auto Qry = fmt::format("SELECT * FROM {} WHERE {} = '{}';", USERS, USER_ID, "LAST_INSERT_ID()");
            this->db.singleSELECT(Qry, response[USER_INFO]);
            response[OUTCOME] = TRUEE;
            return;
        }
        response[OUTCOME] = FALSEE;

    }

    void handleLogin(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM {} WHERE {} = '{}';", USERS, USERNAME, request[USERNAME].asString());
        this->db.SELECT(Qry, response);
        if (response[PASSWORD] == request[PASSWORD])
        {
            response[OUTCOME] = TRUEE;
            return;
        }
        response.clear();
        response[OUTCOME] = FALSEE;        
        return;
    }

    void handleUserSearch(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto Qry = fmt::format("SELECT * FROM {} WHERE username LIKE {} OR name LIKE {}", USERS, request[SEARCH_PHRASE].asString(), request[SEARCH_PHRASE].asString());
        Json::Value search_result;
        this->db.SELECT(Qry, search_result);
        response[SEARCH_RESULT] = search_result;
        response[OUTCOME] = true;
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
            response[OUTCOME] = TRUEE;
            return;
        }
        response[OUTCOME] = FALSEE;
    }

    void handleNewMessage(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        auto query = fmt::format("INSERT INTO {}({}, {}) VALUES ('{}', '{}');",
                        MESSAGES, ENV_ID, OWNER_ID, request[ENV_ID].asInt(), this->m_user_id);
        bool successful;
        response[DETAILS] = this->db.executeQry(query, successful);
        if (successful)
        {
            auto Qry = fmt::format("SELECT * FROM {} WHERE {} = {};", MESSAGES, MESSAGE_ID, "LAST_INSERT_ID()");
            this->db.singleSELECT(Qry, response[USER_INFO]);
            response[OUTCOME] = TRUEE;
            return;
        }
        response[OUTCOME] = FALSEE;

    }

    void addContact(JsonObj& request, JsonObj& response)
    {
        using namespace KeyWords;
        bool successful;
        auto query = fmt::format("INSERT INTO {}({}, {}) VALUES ('{}', '{}');",
                            CONTACTS, USER_ID, CONTACT_ID, request[USER_ID].asString(),
                            request[CONTACT_ID].asString());
        
        response[DETAILS] = this->db.executeQry(query, successful); 
        response[OUTCOME] = successful ? TRUEE : FALSEE;
        return;
    }   

    void getContacts(JsonObj& request, JsonObj& response)
    {
        
    }

};

