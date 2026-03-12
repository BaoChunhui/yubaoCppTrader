cmake_minimum_required(VERSION 3.14)
project(logcfg_demo LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

set(YCT_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
file(GLOB YAMLCPP_SRCS ${YCT_SRC}/YCTUtils/yamlcpp/*.cpp)

include(FetchContent)
FetchContent_Declare(rapidjson GIT_REPOSITORY https://github.com/Tencent/rapidjson.git GIT_TAG v1.1.0)
FetchContent_Populate(rapidjson)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(spdlog CONFIG REQUIRED)

add_executable(logcfg_demo main.cpp
  ${YCT_SRC}/YCTTools/YCTLogger.cpp
  ${YCT_SRC}/YCTUtils/YCTCfgLoader.cpp
  ${YAMLCPP_SRCS})
target_include_directories(logcfg_demo PRIVATE
  ${YCT_SRC}
  ${YCT_SRC}/YCTUtils/yamlcpp
  ${rapidjson_SOURCE_DIR}/include)
target_link_libraries(logcfg_demo PRIVATE Boost::filesystem spdlog::spdlog)
