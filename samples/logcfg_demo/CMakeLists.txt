cmake_minimum_required(VERSION 3.14)
project(logcfg_demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(YCT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
file(GLOB YAMLCPP_SRCS ${YCT_SRC}/YCTUtils/yamlcpp/*.cpp)

find_path(RAPIDJSON_INCLUDE_DIR NAMES rapidjson/document.h)
if(NOT RAPIDJSON_INCLUDE_DIR)
  message(FATAL_ERROR "rapidjson not found. Install it (e.g. apt install rapidjson-dev) or set RAPIDJSON_INCLUDE_DIR.")
endif()

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(spdlog CONFIG REQUIRED)

add_executable(logcfg_demo main.cpp
  ${YCT_SRC}/YCTTools/YCTLogger.cpp
  ${YCT_SRC}/YCTUtils/YCTCfgLoader.cpp
  ${YAMLCPP_SRCS})
target_include_directories(logcfg_demo PRIVATE
  ${YCT_SRC}
  ${YCT_SRC}/YCTUtils/yamlcpp
  ${RAPIDJSON_INCLUDE_DIR})
target_link_libraries(logcfg_demo PRIVATE Boost::filesystem spdlog::spdlog)
