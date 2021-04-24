#pragma once
#include <string>

namespace kipod::ImageProcessing{

struct Algorithm{
    std::string name_;
    void* data_ = nullptr;
};
}
