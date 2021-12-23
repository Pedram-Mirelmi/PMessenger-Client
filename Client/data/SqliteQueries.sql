DROP TABLE IF EXISTS contacts;
CREATE TABLE IF NOT EXISTS contacts
(
    user_id             INTEGER UNSIGNED PRIMARY KEY,
    username            VARCHAR(63) NOT NULL UNIQUE ,
    name                VARCHAR(63) NOT NULL ,
    name_saved          VARCHAR(63) NOT NULL
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
    env_id              INTEGER UNSIGNED PRIMARY KEY,
    first_person        INTEGER UNSIGNED NOT NULL ,
    second_person       INTEGER UNSIGNED NOT NULL ,
    UNIQUE (first_person, second_person),
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id),
    FOREIGN KEY (first_person) REFERENCES contacts (user_id),
    FOREIGN KEY (second_person) REFERENCES contacts (user_id)
);

DROP TABLE IF EXISTS messages;
CREATE TABLE IF NOT EXISTS messages
(
    message_id          INTEGER UNSIGNED PRIMARY KEY ,
    owner_id            INTEGER UNSIGNED NOT NULL ,
    env_id              INTEGER UNSIGNED NOT NULL ,
    created_at          DATETIME DEFAULT CURRENT_TIMESTAMP NOT NULL ,
    FOREIGN KEY (owner_id) REFERENCES contacts (user_id),
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id)
);

DROP TABLE IF EXISTS text_messages;
CREATE TABLE text_messages
(
    message_id          INTEGER UNSIGNED PRIMARY KEY ,
    message_text        TEXT NOT NULL,
    FOREIGN KEY (message_id) REFERENCES messages(message_id)
);

CREATE VIEW text_messages_view AS
    SELECT
           m.message_id,
           m.owner_id,
           m.env_id,
           m.created_at,
           tm.message_text
FROM messages m
INNER JOIN text_messages tm
    ON m.message_id = tm.message_id
