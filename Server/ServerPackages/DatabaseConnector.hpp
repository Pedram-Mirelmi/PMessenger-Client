#pragma once
#include <list>
#include <pqxx/pqxx>

#include <jsoncpp/json/json.h>
#include <mutex>
#include "../../Commons/stringTools.hpp"
#include "../../Commons/KeyWords.hpp"

#ifndef DB_NUMERIC_TYPE 
#define DB_NUMERIC_TYPE 20
#endif

typedef Json::Value msg_t;
typedef unsigned long int id_T;
typedef Json::Value JsonObj;
typedef Json::Value JsonArr;

// typedef std::unordered_set<std::string> set;


class DBConnector
{
private:
    pqxx::connection m_db_connection;
public:

    DBConnector()
        :m_db_connection("dbname = MessengerDB user = postgres password = root "
                         "hostaddr = 127.0.0.1 port = 5432")
    {
        if(this->m_db_connection.is_open())
            std::cout << "Successfuly connected to DB" << std::endl;
        else
        {
            std::cerr << "DB didn't open!!!" << std::endl;
            throw std::runtime_error("DB didn't open!!!");
        }
    }
    DBConnector(const DBConnector&) = delete;

    bool execTransactionQuery(const std::string& query)
    {
        try
        {
            pqxx::work work(this->m_db_connection);
            work.exec(query);
            work.commit();
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }   
    }

    bool singleSELECT(const std::string& query, JsonObj& result)
    {
        std::vector<const char*> columns;
        try
        {
            pqxx::nontransaction nontrans(this->m_db_connection);
            pqxx::result db_result(nontrans.exec(query));
            const pqxx::row& the_row = db_result[0];
            this->setCurrentColumns(db_result, columns);

            for (unsigned int i = 0; i < db_result.columns(); i++)
                if (!the_row[i].is_null())
                    if(the_row[i].type() != DB_NUMERIC_TYPE)
                        result[columns[i]] = the_row[i].c_str();
                    else
                        result[columns[i]] = (Json::Value::Int64)the_row[i].as<int64_t>();
                else
                    result[columns[i]] = Json::nullValue;                
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return false;
        }
        
    };
 
    bool SELECT(const std::string& query,
                JsonArr& result,
                const bool& header = true)
    {
        std::vector<const char*> columns;
        try
        {
            pqxx::nontransaction nontrans(this->m_db_connection);
            pqxx::result db_result(nontrans.exec(query));
            this->setCurrentColumns(db_result, columns);
            if(header)
                for (const auto& column_name : columns)
                    result[0].append(column_name);
                    
            for (const auto& row : db_result)
            {
                JsonObj new_record;
                for (unsigned int i = 0; i < db_result.columns(); i++)
                    if (!row[i].is_null())
                        if(row[i].type() != DB_NUMERIC_TYPE)
                            new_record.append(row[i].c_str());
                        else
                            new_record.append((Json::Value::Int64)row[i].as<int64_t>());
                    else
                        new_record.append(Json::nullValue);
                result.append(new_record);
            }
            return true;
        }
        catch (const std::exception& e)
        {
           std::cerr << e.what() << std::endl;
           return false;
        }
    };

    uint64_t insertPrivateChatBetween(const uint64_t& first, 
                                      const uint64_t& second)
    {
        using namespace KeyWords;
        auto create_env_Qry = "INSERT INTO chat_envs DEFAULT "
                              "VALUES RETURNING env_id AS id;";

        auto inserted_id = this->insertAndreturnLastId(create_env_Qry);
  
        auto create_private_chat_query = fmt::format(
            "INSERT INTO private_chats(env_id, first_person, second_person) "
            "VALUES                     ({},            {},         {});",
                                        inserted_id,    first,      second);
        auto add_one = fmt::format("INSERT INTO chat_attends(user_id, env_id) "
                                        "VALUES ({},    {});",
                                                first, inserted_id);
        auto add_other = fmt::format("INSERT INTO chat_attends(user_id, env_id) "
                                    "VALUES ({},      {});",
                                             second,  inserted_id);

        this->execTransactionQuery(create_private_chat_query);
        this->execTransactionQuery(add_one);
        this->execTransactionQuery(add_other);
        return inserted_id;
    }   
    
    uint64_t insertNewTextMessage(uint64_t owner_id, uint64_t env_id, const char* message_text)
    {
        using namespace KeyWords;
        auto create_message_query = fmt::format("INSERT INTO messages(env_id, owner_id) "
                                                "VALUES({},     {});",
                                                        env_id, owner_id);
        if (this->execTransactionQuery(create_message_query))
        {
            auto created_message_id = this->getLastInsertId("messages", "message_id");
            auto create_text_message_query = fmt::format("INSERT INTO text_messages(message_id, message_text) "
                                                         "VALUES({},                 {}",
                                                                created_message_id, toRaw(message_text));

            return  this->execTransactionQuery(create_message_query) ? created_message_id : 0;
        }
        return 0;
    }   

    uint64_t insertNewUser(const char* username, const char* password)
    {
        using namespace KeyWords;
        auto query = fmt::format("INSERT INTO users(username, name, password)"
                                 "VALUES ('{}', '{}',  '{}');",
                                  username, username, password);                                 
        return this->execTransactionQuery(query) ? this->getLastInsertId("users", "user_id") : 0;
    }

    void setPrivateEnvsListThatUserAttends(JsonArr& envs_list,
                                           const uint64_t& user_id) // ToDo
    {
        JsonArr envs;
        auto privates_query = fmt::format("SELECT env_id from private_chats_view "
                                          "WHERE first_person = {} OR second_person = {};",
                                           user_id, user_id);
        this->SELECT(privates_query, envs, false);
        for (const auto& single_item_list : envs)
            envs_list.append(single_item_list[0]);
    }

protected:  
    uint64_t getLastInsertId(const char* tablename, const char* id_field)
    {
        JsonObj result;
        auto query = fmt::format("SELECT currval(pg_get_serial_sequence('{}', '{}')) AS id;"
                                                                , tablename, id_field);
        // auto query = fmt::format("SELECT last_value FROM {}_{}_seq AS id;"
        //                                         , tablename, id_field);
        
        this->singleSELECT(query, result);
        std::cout << result << std::endl;
        return stoi(result["id"].asString());
    }

    uint64_t insertAndreturnLastId(const char* query)
    {
        JsonObj insert_result;
        try
        {        
            this->singleSELECT(query, insert_result);
            auto id = stoi(insert_result["id"].asString());
            std::cout << insert_result << std::endl;
            return id;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return 0;
        }
    }

private:
    inline void setCurrentColumns(const pqxx::result& result_set,
                                  std::vector<const char*>& columns)
    {
        columns.clear();
        columns.reserve(result_set.columns());
        for (unsigned int i = 0; i < result_set.columns(); i++)
            columns.emplace_back(result_set.column_name(i));
    }


};
