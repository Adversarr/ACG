CPMAddPackage(
  NAME fmt
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 9.1.0
)

CPMAddPackage(
  NAME spdlog
  GITHUB_REPOSITORY gabime/spdlog
  VERSION 1.10.0
  OPTIONS "SPDLOG_FMT_EXTERNAL ON"
)