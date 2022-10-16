#pragma once

#include <spdlog/spdlog.h>


#ifndef NDEBUG
#define ACG_DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#else
#define ACG_DEBUG_LOG(...)
#endif
