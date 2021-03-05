#pragma once
#include "../core.h"

namespace kipod{

class IBuffer{
    public:
    IBuffer() = default;
    IBuffer(unsigned int count, unsigned int size) : count_(count), size_(size){}
    virtual ~IBuffer() = default;
    unsigned int id_ = 0;
    unsigned int count_ = 0;
    unsigned int size_ = 0;
};

class Buffer : public IBuffer{
protected:
    void* data_ = nullptr;

public:
    Buffer() = default;
    Buffer(void* data, unsigned int count, unsigned int size) : IBuffer(count, size), data_(data){}
    virtual ~Buffer() = default;
    virtual void Bind()=0;
};

}
