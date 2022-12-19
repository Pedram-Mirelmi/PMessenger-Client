#pragma once
#include <memory>

class IService : public std::enable_shared_from_this<IService>
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
};
