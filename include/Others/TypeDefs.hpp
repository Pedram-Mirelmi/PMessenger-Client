#pragma once
#include <memory>
#include "../NetworkPackages/NetMessages/NetMessageType.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../AsyncNetFramework/io/INetMessageBodyDeserializer.h"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

typedef BasicNetMessage<NetMessageType> _NetMsg;
typedef shared_ptr<_NetMsg> _NetMsg_ptr;

typedef INetMessageBodyDeserializer<NetMessageType> _IMsgDeserializer;
typedef shared_ptr<_IMsgDeserializer> _IMsgDeserializer_ptr ;

typedef QHash<QString, QVariant> InfoContainer;
typedef std::shared_ptr<InfoContainer> InfoContainerPtr;

typedef QVector<InfoContainer> InfoCollection;
typedef std::shared_ptr<InfoCollection> InfoCollectionPtr;

typedef QJsonObject NetInfoContainer;
typedef std::shared_ptr<NetInfoContainer> NetInfoContainerPtr;

typedef QJsonArray NetInfoCollection;
typedef std::shared_ptr<QJsonArray> NetInfoCollectionPtr;


typedef QVariantMap QmlObject;
