#pragma once
#include <iostream>
#define DEBUG_MODE

#ifdef DEBUG_MODE
    #define DEBUG(TEXT) (std::cout << "[DEBUG] " << TEXT << std::endl)
#else
    #define DEBUG(TEXT)
#endif
#define INFO(TEXT) (std::cout << "[INFO] " << TEXT << std::endl)
#define WARN(TEXT) (std::cout << "[WARN] " << TEXT << std::endl)
#define ERROR(TEXT) (std::cerr << "[ERROR] " << TEXT << std::endl)
#define FATAL(TEXT) (std::cerr << "[FATAL] " << TEXT << std::endl)