find_package(Eigen3 3.4 NO_MODULE)

if (NOT ${EIGEN3_FOUND}) 
  # Get Eigen.
  message(NOTICE "System Eigen Not found. Download from gitlab")
  CPMAddPackage(
    NAME Eigen3
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
    GIT_TAG 3.4.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    OPTIONS "EIGEN_BUILD_DOC OFF" "BUILD_TESTING OFF" "EIGEN_BUILD_PKGCONFIG OFF"
  )
endif()

message(STATUS "Eigen Found. Version: ${EIGEN3_VERSION_STRING} at ${EIGEN3_ROOT_DIR}")

