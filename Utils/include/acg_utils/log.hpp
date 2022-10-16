#pragma once

#include <spdlog/spdlog.h>


namespace acg::utils::details {

class FunctionDeferredLogger {
private:
  const char* function_name_;

public:
  inline explicit FunctionDeferredLogger(const char* fname,
                                         const char* file,
                                         const int line): function_name_{fname} {
    spdlog::debug("Function Call: <{}> at {}:{}", fname, file, line);
  }


  inline ~FunctionDeferredLogger() {
    spdlog::debug("Function Exiting. <{}>", function_name_);
  }

};

}


#ifndef NDEBUG
#define ACG_DEBUG_LOG(...) spdlog::debug(__VA_ARGS__)
#define LOG_FUNCTION_CALL_DEBUG \
    acg::utils::details::FunctionDeferredLogger ___(__PRETTY_FUNCTION__, __FILE__, __LINE__)
#else
#define ACG_DEBUG_LOG(...)

#define LOG_FUNCTION_CALL_DEBUG
#endif

