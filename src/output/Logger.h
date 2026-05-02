#pragma once

#include <iostream>

#define ENABLE_LOGGING

// TODO: this file should contain a macro for logging things (or define it and implement in .cpp?)
#ifdef ENABLE_LOGGING
#define LOG(msg) std::cout << msg << "\n"
#else
#define LOG(msg)
#endif