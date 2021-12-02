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


private:
    inline void setCurrentColumns(msg_t& columns_list, sql::ResultSet* result_set)
    {
        auto meta_data = result_set->getMetaData();
        auto n = meta_data->getColumnCount();
        for (unsigned int i = 1; i <= n; i++)
            columns_list.append(meta_data->getColumnName(i).c_str());
    }

};
