DROP VIEW IF EXISTS text_messages_view;
DROP VIEW IF EXISTS private_chats_view;
DROP TABLE IF EXISTS text_messages;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS private_chats;
DROP TABLE IF EXISTS chat_attends;
DROP TABLE IF EXISTS chat_envs;
DROP TABLE IF EXISTS contacts;
DROP TABLE IF EXISTS users;


DROP TABLE IF EXISTS users;
CREATE TABLE users 
(
    user_id             BIGSERIAL NOT NULL PRIMARY KEY,
    username            VARCHAR(200) UNIQUE NOT NULL,
    password            VARCHAR(200) NOT NULL ,
    name                VARCHAR(200),
    created_at          DATE NOT NULL DEFAULT(NOW())
);

DROP TABLE IF EXISTS contacts;
CREATE TABLE contacts
(
    user_id             BIGINT NOT NULL ,
    contact_id          BIGSERIAL NOT NULL ,
    PRIMARY KEY (user_id, contact_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE CASCADE ,
    FOREIGN KEY (contact_id) REFERENCES users(user_id) ON DELETE CASCADE
);

DROP TABLE IF EXISTS chat_envs;
CREATE TABLE chat_envs
(
    env_id          BIGSERIAL NOT NULL PRIMARY KEY ,
    created_at      DATE NOT NULL DEFAULT NOW()           
);

CREATE TABLE private_chats
(
    env_id          BIGINT PRIMARY KEY ,
    first_person    BIGINT NOT NULL ,
    second_person   BIGINT NOT NULL ,
    UNIQUE (first_person, second_person),
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id),
    FOREIGN KEY (first_person) REFERENCES users(user_id),
    FOREIGN KEY (second_person) REFERENCES users(user_id)
);


CREATE TABLE chat_attends
(
    user_id         BIGINT NOT NULL ,
    env_id          BIGINT NOT NULL ,
    PRIMARY KEY (user_id, env_id),
    FOREIGN KEY (user_id) REFERENCES users(user_id) ON DELETE NO ACTION ,
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id) ON DELETE CASCADE
);

CREATE TABLE messages
(
    message_id      BIGSERIAL NOT NULL PRIMARY KEY ,
    env_id          BIGINT NOT NULL ,
    owner_id        BIGINT NOT NULL ,
    created_at      TIMESTAMP NOT NULL DEFAULT NOW(),
    FOREIGN KEY(owner_id) REFERENCES users(user_id) ON DELETE CASCADE,
    FOREIGN KEY (env_id) REFERENCES chat_envs(env_id) ON DELETE CASCADE
);

CREATE TABLE text_messages
(
    messages_id     BIGINT NOT NULL PRIMARY KEY ,
    message_text    TEXT NOT NULL ,
    FOREIGN KEY (messages_id) REFERENCES messages(message_id)
);

CREATE VIEW text_messages_view AS
    SELECT
        m.message_id,
        m.owner_id,
        m.env_id,
        m.created_at
    FROM messages m
    LEFT JOIN text_messages tm
        ON m.message_id = tm.messages_id;

CREATE VIEW private_chats_view AS
    SELECT
        ce.env_id,
        ce.created_at,
        pc.first_person,
        pc.second_person
    FROM chat_envs ce
    LEFT JOIN private_chats pc
        ON ce.env_id = pc.env_id;

