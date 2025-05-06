#pragma once
#include <iostream>
#define DEBUG_MODE

#define DEBUG(TEXT)
#ifdef DEBUG_MODE
    #define DEBUG(TEXT) std::cout << "[DEBUG] " << TEXT << std::endl
#endif
#define INFO(TEXT) std::cout << "[INFO] " << TEXT << std::endl
#define ERROR(TEXT) std::cerr << "[ERROR] " << TEXT << std::endl
#define FATAL(TEXT) std::cerr << "[FATAL] " << TEXT << std::endl