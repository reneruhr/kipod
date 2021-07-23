#pragma once
#include <filesystem>
#include "log.h"
#include <iostream>


inline auto PrintPath() -> void
{
    //std::cout << std::filesystem::current_path() << "\n";
    LOG("Current path: {}", std::filesystem::current_path());
}
