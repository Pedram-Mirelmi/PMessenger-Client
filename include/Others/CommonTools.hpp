#pragma once
#include "./TypeDefs.hpp"
#include <QJsonObject>
#include <QJsonArray>


void
convertToHash(InfoContainer &target,
              const QJsonObject &source);


NetInfoCollectionPtr
convertToNormalForm(const QJsonArray &data);;

