#pragma once
#include <utility>
#include <memory>

namespace kipod
{

auto MakeHash(std::string name) -> std::size_t
{
    static auto hash = std::hash<std::string>();
    return hash(std::move(name));
}

}