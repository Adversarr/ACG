#pragma once
#include <spdlog/spdlog.h>
#include "log.hpp"
namespace acg::utils {

  // TODO: New implementation.
class FunctionDeferredLogger {
private:
  const char* function_name_;

public:
  inline explicit FunctionDeferredLogger(const char* fname, const char* file, const int line)
      : function_name_{fname} {
    ACG_DEBUG_LOG("Function Call: <{}> at {}:{}", fname, file, line);
  }

  inline ~FunctionDeferredLogger() { spdlog::debug("Function Exiting. <{}>", function_name_); }
};

}
