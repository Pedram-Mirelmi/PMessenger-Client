#pragma once

#include <memory>

using std::shared_ptr;

class ISerializable : public std::enable_shared_from_this<ISerializable>
{
public:
    virtual void deserialize(char* buffer) = 0;

    virtual void serialize(char* buffer) const = 0;

    virtual uint32_t calculateNeededSizeForThis() const = 0;
};
