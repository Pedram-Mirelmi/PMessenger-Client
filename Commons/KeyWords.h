#pragma once
#include <iostream>
#include <string>

//
//enum class KeyWord : unsigned int
//{
//    SELECT ,
//    INSERT ,
//    UPDATE ,
//    INTO ,
//    VALUES ,
//    SET ,
//    FROM ,
//    WHERE ,
//    JOIN ,
//    ON ,
//    OR ,
//    USING ,
//
//    // Database tables
//    USERS ,
//
//
//    // Request types
//    REQUEST_TYPE,
//    LOGIN,
//    REGISTER,
//    CLOSE_CONNECTION,
//    OUTCOME,
//    MESSAGE,
//    ANNOUNCE_ONLINE,
//    NEW_MESSAGE,
//
//
//    // Other Request keywords
//    TIME,
//    USERNAME,
//    PASSWORD,
//    NAME,
//    USER_ID,
//    GENDER,
//    BIO,
//    PHONE,
//    MESSAGE_INFO,
//    MUTUAL,
//    CHAR_ENV,
//
//
//
//    TRUEE,
//};

//bool operator==(KeyWord keyword, const string& str)
//{
//    return str.size() && (char)keyword == str[0];
//}


namespace KeyWords
{
//    using namespace std::literals;
// Database keywords
constexpr auto SELECT = "SELECT";
constexpr auto INSERT = "INSERT";
constexpr auto UPDATE = "UPDATE";
constexpr auto INTO = "INTO";
constexpr auto VALUES = "VALUES";
constexpr auto SET = "SET";
constexpr auto FROM = "FROM";
constexpr auto WHERE = "WHERE";
constexpr auto JOIN = "JOIN";
constexpr auto ON = "ON";
constexpr auto OR = "OR";
constexpr auto USING = "USING";

// Database tables
constexpr auto USERS = "users";
constexpr auto CONTACTS = "contacts";
constexpr auto CHAT_ENVS = "chat_envs";
constexpr auto CHAT_ATTENDS = "chat_attends";
constexpr auto MESSAGES = "messages";
constexpr auto PRIVATE_CHAT = "private_chat";
constexpr auto PRIVATE_CHATS = "private_chats";
constexpr auto FIRST_PERSON = "first_person";
constexpr auto SECOND_PERSON = "second_person";






// Request types
constexpr auto MESSAGE_TYPE = "message_type";
constexpr auto LOGIN = "login";
constexpr auto REGISTER = "register";
constexpr auto USER_SEARCH_RESULT = "user_search_result";
constexpr auto USER_SEARCH = "user_search";
constexpr auto FETCH = "fetch";
constexpr auto FETCH_ALL = "fetch_all";

constexpr auto CREATE_NEW_CHAT = "create_new_chat";


constexpr auto OUTCOME = "outcome";
constexpr auto CLOSE_CONNECTION = "close_connection";
constexpr auto MESSAGE = "message";

// response types
constexpr auto ENTERING = "entering";
constexpr auto DETAILS = "details";
constexpr auto CHAT_CREATION_CONFIRMATION = "chat_creation_confirmation";
constexpr auto MESSAGE_SENT_CONFIRMATION = "message_sent_confirmation";


constexpr auto DATA_SUBJECT = "data_subject";


// message details keywords
constexpr auto TIME = "time";
constexpr auto USERNAME = "username";
constexpr auto PASSWORD = "password";
constexpr auto NAME = "name";
constexpr auto USER_ID = "user_id";
constexpr auto CONTACT_ID = "contact_id";
constexpr auto GENDER = "gender";
constexpr auto BIO = "bio";
constexpr auto PHONE = "phone";
constexpr auto MESSAGE_INFO = "message_info";
constexpr auto MUTUAL = "mutual";
constexpr auto CHAT_ENV = "chat_env";
constexpr auto CHAT_ENV_DETAILS_RESULT = "chat_env_details_result";
constexpr auto USER_INFO = "user_info";
constexpr auto SUCCESS = "success";
constexpr auto SEARCH_PHRASE = "search_phrase";
constexpr auto SEARCH_RESULT = "search_result";
constexpr auto DATA = "data";
constexpr auto DATA_DETAILS = "data_details";
constexpr auto CHAT_ENV_TYPE = "chat_env_type";


constexpr auto FETCH_RESULT = "fetch_result";
constexpr auto FETCH_ALL_RESULT = "fetch_all_result";
constexpr auto ALL_CHAT_ENV_IDS = "all_chat_env_ids";
constexpr auto GET_CHAT_ENV_DETAILS = "get_chat_env_details"

constexpr auto CREATED_AT = "created_at";
constexpr auto MESSAGE_TEXT = "message_text";\
constexpr auto SEEN = "seen";
constexpr auto ENV_ID = "env_id";
constexpr auto OWNER_ID = "owner_id";
constexpr auto MESSAGE_ID = "message_id";







constexpr bool TRUEE = true;
constexpr bool FALSEE = false;

constexpr auto ERROR_NO = "error_no";
constexpr auto E1 = "Wrong password!";
constexpr auto E2 = "Wrong username";

} // namespace KeyWords

