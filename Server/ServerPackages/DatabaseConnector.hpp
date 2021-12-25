#pragma once
#include <list>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>
#include <jsoncpp/json/json.h>
#include <mutex>

typedef Json::Value QryRes;
typedef Json::Value msg_t;
typedef unsigned long int id_T;
// typedef std::unordered_set<std::string> set;

static std::mutex db_lock;
static sql::Driver* ThsGetDriverInstance()
{ 
    std::lock_guard<std::mutex> lock(db_lock);
    return get_driver_instance();
} 

class DBConnector
{
private:
    sql::Driver* driver;
    sql::Connection* connection;
    sql::Statement* statement;
public:

    DBConnector()
    {
        this->driver = ThsGetDriverInstance();
        connection = driver->connect("localhost", "root", "root");
        connection->setSchema("Telegram");
        statement = connection->createStatement();
    }
    DBConnector(const DBConnector&) = delete;

    std::string executeQry(const std::string& query, bool& success)
    {
        try
        {
            statement->execute(query);
            success = true;
            return std::string("Successfully executed") + query;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Couldn't execute query <" << query << "> ====> " << e.what() << '\n';
            success = false;
            return e.what();
        }   
    }

    void singleSELECT(const std::string& query, QryRes& result)
    {
        Json::Value columns;
        try
        {
            auto result_set = statement->executeQuery(query);
            setCurrentColumns(columns, result_set);
            if(result_set->next())
                for (uint8_t i = 1; i <= columns.size(); i++)
                    result[columns[i - 1].asString()] = result_set->getString(i).c_str();
            delete result_set;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    };
 
    std::string SELECT(const std::string& query, QryRes& result)
    {
        Json::Value columns;
        try
        {
            auto result_set = statement->executeQuery(query);
            setCurrentColumns(columns, result_set);
            result.append(columns);
            while (result_set->next())
            {
                Json::Value new_row;
                for (uint32_t i = 1; i <= columns.size(); i++)
                    new_row.append(result_set->getString(i).c_str());
                result.append(new_row);
            }
            delete result_set;
            return fmt::format("succesfullt executed {}", query); 
        }
        catch (const std::exception& e)
        {
           return e.what();
        }
    };

    uint64_t insertPrivateChatBetween(uint64_t first, uint64_t second)
    {
        using namespace KeyWords;
        auto create_env_Qry = "INSERT INTO chat_envs() VALUES ();";
        auto created_env_id = this->getLastInsertId();
        auto create_private_chat_query = fmt::format("INSERT INTO private_chats(env_id, first_person, second_person) "
                                                     "VALUES ({},            {},         {});",
                                                             created_env_id, first,      second);
        auto add_creator = fmt::format("INSERT INTO chat_attends(user_id, env_id) "
                                        "VALUES ({},    {});",
                                                first, created_env_id);
        auto add_other = fmt::format("INSERT INTO chat_attends(user_id, env_id) "
                                     "VALUES ({},     {});",
                                              second, created_env_id);

        bool successful1, successful2, successful3;
        
        this->executeQry(create_env_Qry, successful1);
        this->executeQry(add_creator, successful2);
        this->executeQry(add_other, successful3);
        
        return successful1 && successful2 && successful3 ? created_env_id : 0;
    }

    uint64_t insertNewTextMessage(uint64_t owner_id, uint64_t env_id, const char* message_text)
    {
        using namespace KeyWords;
        auto create_message_query = fmt::format("INSERT INTO messages(env_id, owner_id) "
                                                "VALUES({},     {});",
                                                                                               env_id, owner_id);
        bool succusfull1, succesfull2;
        this->executeQry(create_message_query, succusfull1);
        if (succusfull1)
        {
            auto created_message_id = this->getLastInsertId();
            auto create_text_message_query = fmt::format("INSERT INTO text_messages(message_id, message_text) "
                                                        "VALUES({},                 {}",
                                                                created_message_id, toRaw(message_text));
            this->executeQry(create_message_query, succesfull2);
            return succesfull2 ? created_message_id : 0;
        }
        return 0;
    }

    uint64_t insertNewUser(const char* username, const char* password)
    {
        using namespace KeyWords;
        auto query = fmt::format("INSERT INTO users(username, password)"
                                 "VALUES ('{}',      '{}');",
                                           username, password);
       
        bool successful;
        this->executeQry(query, successful);
        return successful ? this->getLastInsertId() : 0;
    }

protected:

    uint64_t getLastInsertId()
    {
        JsonObj result;
        this->singleSELECT("SELECT LAST_INSERT_ID() AS id;", result);
        return result["id"].asUInt64();
    }

private:
    inline void setCurrentColumns(msg_t& columns_list, sql::ResultSet* result_set)
    {
        auto meta_data = result_set->getMetaData();
        auto n = meta_data->getColumnCount();
        for (unsigned int i = 1; i <= n; i++)
            columns_list.append(meta_data->getColumnName(i).c_str());
    }


};
