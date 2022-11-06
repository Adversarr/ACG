#pragma once

namespace acg::utils {

// TODO: do not use standard spdlog.
//
// class FunctionDeferredLogger {
// private:
//   const char* function_name_;

// public:
//   inline explicit FunctionDeferredLogger(const char* fname, const char* file, const int line)
//       : function_name_{fname} {
//     spdlog::debug("Function Call: <{}> at {}:{}", fname, file, line);
//   }

//   inline ~FunctionDeferredLogger() { spdlog::debug("Function Exiting. <{}>", function_name_); }
// };

}
