#pragma once
#include "./TypeDefs.hpp"
#include <QJsonObject>
#include <QJsonArray>


InfoContainer
convertToHash(const NetInfoContainer &source);


NetInfoCollectionPtr
convertToNormalForm(const QJsonArray &data);;

