DROP TABLE IF EXISTS users;
CREATE TABLE IF NOT EXISTS users
(
    user_id             INTEGER UNSIGNED PRIMARY KEY,
    username            VARCHAR(63) NOT NULL UNIQUE ,
    name                VARCHAR(63) NOT NULL ,
    name_saved          VARCHAR(63)
);

DROP TABLE IF EXISTS chat_envs;
CREATE TABLE IF NOT EXISTS chat_envs
(
    env_id              INTEGER UNSIGNED PRIMARY KEY,
    participates        INTEGER UNSIGNED NOT NULL
);


DROP TABLE IF EXISTS private_chats;
CREATE TABLE IF NOT EXISTS private_chats
(
    env_id                  INTEGER UNSIGNED PRIMARY KEY,
    other_person_id            INTEGER UNSIGNED UNIQUE,
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id),
    FOREIGN KEY (other_person_id) REFERENCES users (user_id)
);

DROP TABLE IF EXISTS pending_chat_envs;
CREATE TABLE IF NOT EXISTS pending_chat_envs
(
    invalid_env_id          INTEGER PRIMARY KEY AUTOINCREMENT,
    env_type                CHAR(1),
    other_person_id         INTEGER UNSIGNED UNIQUE ,
    FOREIGN KEY (other_person_id) REFERENCES users(user_id)
);

DROP TABLE IF EXISTS messages;
CREATE TABLE IF NOT EXISTS messages
(
    message_id                  INTEGER UNSIGNED PRIMARY KEY ,
    owner_id                    INTEGER UNSIGNED NOT NULL ,
    env_id                      INTEGER UNSIGNED NOT NULL ,
    created_at                  DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL ,
    FOREIGN KEY (owner_id) REFERENCES users (user_id),
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id)
);

DROP TABLE IF EXISTS text_messages;
CREATE TABLE text_messages
(
    message_id          INTEGER UNSIGNED PRIMARY KEY ,
    message_text        TEXT NOT NULL,
    FOREIGN KEY (message_id) REFERENCES messages(message_id)
);

DROP TABLE IF EXISTS pending_text_messages;
CREATE TABLE IF NOT EXISTS pending_text_messages
(
    invalid_message_id        INTEGER PRIMARY KEY AUTOINCREMENT,
    env_id                    INTEGER UNSIGNED ,
    invalid_env_id            INTEGER UNSIGNED ,
    message_text              TEXT NOT NULL,
    FOREIGN KEY (invalid_message_id) REFERENCES pending_chat_envs(invalid_env_id),
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id)
);

CREATE VIEW IF NOT EXISTS text_messages_view AS
    SELECT
           m.message_id,
           m.owner_id,
           m.env_id,
           m.created_at,
           tm.message_text
FROM messages m
INNER JOIN text_messages tm
    ON m.message_id = tm.message_id;
    
