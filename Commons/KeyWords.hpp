#pragma once
#include <iostream>

// net message types should be converted into an enum class later for perfomance in server

namespace KeyWords
{
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Database tables
// constexpr auto USERS = "users";
// constexpr auto CONTACTS = "contacts";
// constexpr auto CHAT_ENVS = "chat_envs";
// constexpr auto CHAT_ATTENDS = "chat_attends";
// constexpr auto MESSAGES = "messages";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tables keywords
constexpr auto FIRST_PERSON = "first_person";
constexpr auto SECOND_PERSON = "second_person";
constexpr auto USERNAME = "username";
constexpr auto PASSWORD = "password";
constexpr auto NAME = "name";
constexpr auto USER_ID = "user_id";
constexpr auto CONTACT_ID = "contact_id";
constexpr auto NAME_SAVED = "name_saved";
constexpr auto CREATED_AT = "created_at";
constexpr auto SEEN = "seen";
constexpr auto ENV_ID = "env_id";
constexpr auto OWNER_ID = "owner_id";
constexpr auto MESSAGE_ID = "message_id";
constexpr auto PHOTO_ID = "photo_id";
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    Request and responess:
*/
constexpr auto NET_MESSAGE_TYPE = "net_message_type"; // in all requests and responses and basically every network message


constexpr auto SUCCESSFUL = "successful"; // in all responses. represents that the response has the information wanted by client or not

constexpr auto DETAILS = "details";     // details of process in case of failiar
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* ENTERING:

REGISTERING:
    request:    {NET_MESSAGE_TYPE: REGISTER, USERNAME: ..., PASSWORD: ...}
    response:   {NET_MESSAGE_TYPE: REGISTER_RESULT, SUCCESFUL: TRUE ? FALSE,  USER_INFO: {...} }

LOGINING:
    request:    {NET_MESSAGE_TYPE: LOGIN, USERNAME: ..., PASSWORD: ... }
    response:   {NET_MESSAGE_TYPE: LOGIN_RESULT, SUCCESFUL: TRUE ? FALSE, USER_INFO: {...}}
*/
constexpr auto LOGIN = "login";
constexpr auto REGISTER = "register";
constexpr auto LOGIN_RESULT = "login_result";
constexpr auto REGISTER_RESULT = "register_result";
constexpr auto USER_INFO = "user_info";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    FETCH after everytime that user get online sends this request to get anything new and update itself.
    for now (this version) we just send the chat environments that user attends with basic informaions about them. next step client will ask for messages in environments with GET_ENV_MESSAGES

    request:    {NET_MESSAGE_TYPE: FETCH}
    response:   {NET_MESSAGE_TYPE: DATA, DATA_TYPE: FETCH_RESULT, PRIVATE_CHATS: [...], GROUP_CHATS: [...], CHANNELS: [...], ...}
*/
constexpr auto FETCH = "fetch";
constexpr auto DATA = "data";
constexpr auto DATA_TYPE = "data_type";
constexpr auto FETCH_RESULT = "fetch_result";
constexpr auto PRIVATE_CHATS = "private_chats";
constexpr auto GROUP_CHATS = "group_chats";
constexpr auto CHANNELS = "channels";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    GET_PRIVATE_ENV_DETAILS when client requests for messages in a private_env with id of "env_id"
    request:    {MESSAGE_TYPE: GET_PRIVATE_ENV_DETAILS, ENV_ID: ...}
    response:   {NET_MESSAGE_TYPE: DATA, DATA_TYPE: ENV_DETAILS, ENV_MESSAGES:[TEXT_MESSAGES: [], ... ], ENV_INFO: {}}
*/
constexpr auto GET_PRIVATE_ENV_DETAILS = "get_private_env_details";
constexpr auto ENV_DETAILS = "env_details";
constexpr auto ENV_MESSAGES = "env_messages";
constexpr auto ENV_INFO = "env_info";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    SEARCH_USERNAME when user search for another user to start a new conversation with
    request:    {NET_MESSAGE_TYPE: SEARCH_USERNAME, USERNAME_TO_SEARCH: ...}
    response:   {NET_MESSAGE_TYPE: USER_SEARCH_RESULT, SEARCH_RESULT: [...]}
]
*/
constexpr auto SEARCH_USERNAME = "username_search";
constexpr auto USERNAME_TO_SEARCH = "username_to_search";
constexpr auto SEARCH_USERNAME_RESULT = "search_username_result";
constexpr auto SEARCH_RESULT = "result";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    CREATE_NEW_PRIVATE_CHAT when user wants to start a new conversation with another using their user_id (got by SEARCH_USERNAME earlier)
    request:    {NET_MESSAGE_TYPE: CREATE_NEW_PRIVATE_CHAT, INVALID_ENV_ID: ..., user_id: ...}
    response:   {NET_MESSAGE_TYPE: DATA, DATA_TYPE: CHAT_CREATION_CONFIRMATION, ENV_INFO: {ENV_TYPE: PRIVATE_CHAT, INVALID_ENV_ID: ..., env_id: ...} }

*/
constexpr auto CREATE_NEW_PRIVATE_CHAT = "create_new_private_chat";
constexpr auto INVALID_ENV_ID = "invalid_env_id";
constexpr auto CHAT_CREATION_CONFIRMATION = "chat_creation_confirmation";
constexpr auto ENV_TYPE = "env_type";
constexpr auto PRIVATE_CHAT = "private_chat";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    NEW_TEXT_MESSAGE when user sends a new message on an "env_id" chat environment
    request:    {NET_MESSAGE_TYPE: NEW_TEXT_MESSAGE, MESSAGE_TEXT: ..., INVALID_MESSAGE_ID: ..., env_id: ...}
    response:   {NET_MESSAGE_TYPE: DATA, DATA_TYPE: MESSAGE_SENT_CONFIRMAION, MESSAGE_INFO: {...}}
*/
constexpr auto NEW_TEXT_MESSAGE = "new_text_message";
constexpr auto MESSAGE_TEXT = "message_text";
constexpr auto INVALID_MESSAGE_ID = "invalid_message_id";
constexpr auto MESSAGE_INFO = "message_info";
constexpr auto MESSAGE_SENT_CONFIRMATION = "message_sent_confirmation";
constexpr auto TEXT_MESSAGE = "text_message";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
    MESSAGE when server wants to deliver any kind of message (seen or not text or picture or ...)
    1-way server message:   {MESSAGE_TYPE: DATA, DATA_TYPE: MESSAGE, TEXT_MESSAGES: [...], ...}
*/
constexpr auto MESSAGE = "message";
constexpr auto TEXT_MESSAGES = "text_messages";
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constexpr auto TIME = "time";
// constexpr auto GENDER = "gender";
// constexpr auto BIO = "bio";
// constexpr auto PHONE = "phone";
// constexpr auto MESSAGE_INFO = "message_info";
// constexpr auto MUTUAL = "mutual";
// constexpr auto USER_INFO = "user_info";
// constexpr auto SUCCESS = "success";


//constexpr auto TEXT_MESSAGE = "text_message";





constexpr auto CLOSE_CONNECTION = "close_connection"; // when client or server want to close the connection.

constexpr auto ERROR_NO = "error_no";
constexpr auto E1 = "Wrong password!";
constexpr auto E2 = "Wrong username";

} // namespace KeyWords
