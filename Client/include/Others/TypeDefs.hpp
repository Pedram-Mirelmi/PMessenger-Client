#pragma once
#include <QHash>
#include <QVariant>
#include <QVariantMap>

typedef QHash<QString, QVariant> InfoContainer;
typedef std::shared_ptr<InfoContainer> InfoContainerPtr;

typedef QVector<InfoContainer> InfoCollection;
typedef std::shared_ptr<InfoCollection> InfoCollectionPtr;

typedef QJsonObject NetInfoContainer;

typedef QVariantMap QmlObject;
