#include "CommonTools.hpp"

InfoContainer convertToHash(const NetInfoContainer &source)
{
    InfoContainer target;
    for (auto itter = source.constBegin(); itter < source.constEnd(); itter++)
        target[itter.key()] = itter.value().toVariant();
    return target;
}

NetInfoCollectionPtr convertToNormalForm(const QJsonArray &data)
{
    auto target = std::make_shared<QJsonArray> ();
    auto fields = data[0].toArray();
    for (auto itter = data.constBegin() + 1; itter < data.constEnd(); itter++)
    {
        QJsonObject new_row;
        const QJsonArray& data_row = itter->toArray();
        for (uint8_t i = 0; i < fields.size(); i++)
            new_row[fields[i].toString()] = data_row[i];
        target->append(new_row);
    }
    return target;
}
