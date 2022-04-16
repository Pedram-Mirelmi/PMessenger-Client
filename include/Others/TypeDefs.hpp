#pragma once
#include <QHash>
#include <QVariant>
#include <QVariantMap>

typedef QHash<QString, QVariant> InfoContainer;
typedef std::shared_ptr<InfoContainer> InfoContainerPtr;

typedef QVector<InfoContainer> InfoCollection;
typedef std::shared_ptr<InfoCollection> InfoCollectionPtr;

typedef QJsonObject NetInfoContainer;
typedef std::shared_ptr<NetInfoContainer> NetInfoContainerPtr;

typedef QJsonArray NetInfoCollection;
typedef std::shared_ptr<QJsonArray> NetInfoCollectionPtr;


typedef QVariantMap QmlObject;
