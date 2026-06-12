#pragma once

#include <iostream>

// TODO: should make a "settings" file where these macros are defined
#define ENABLE_LOGGING_VERBOSE
#define ENABLE_LOGGING_DEBUG
#define ENABLE_LOGGING_INFO
#define ENABLE_LOGGING_WARNING
#define ENABLE_LOGGING_ERROR
#define ENABLE_LOGGING_FATAL


// S-LOG meaning Simple-LOG (or Squid-LOG)
#ifdef ENABLE_LOGGING_VERBOSE
#define SLOG_VERBOSE(msg) std::cerr << "[VERBOSE]  " << msg << "\n"
#else
#define SLOG_VERBOSE(msg)
#endif

#ifdef ENABLE_LOGGING_DEBUG
#define SLOG_DEBUG(msg) std::cerr << "[DEBUG]    " << msg << "\n"
#else
#define SLOG_DEBUG(msg)
#endif

#ifdef ENABLE_LOGGING_INFO
#define SLOG_INFO(msg) std::cerr << "[INFO]     " << msg << "\n"
#else
#define SLOG_INFO(msg)
#endif

#ifdef ENABLE_LOGGING_WARNING
#define SLOG_WARNING(msg) std::cerr << "[WARNING]  " << msg << "\n"
#else
#define SLOG_WARNING(msg)
#endif

#ifdef ENABLE_LOGGING_ERROR
#define SLOG_ERROR(msg) std::cerr << "[ERROR]    " << msg << "\n"
#else
#define SLOG_ERROR(msg)
#endif

#ifdef ENABLE_LOGGING_FATAL
#define SLOG_FATAL(msg) std::cerr << "[FATAL]    " << msg << "\n"
#else
#define SLOG_FATAL(msg)
#endif