cmake_minimum_required(VERSION 3.0)
set(COLOR_VERSION_MAJOR 0)
set(COLOR_VERSION_MINOR 0)
set(COLOR_VERSION_PATCH 0)
set(COLOR_VERSION ${COLOR_VERSION_MAJOR}.${COLOR_VERSION_MINOR}.${COLOR_VERSION_PATCH})
project(color VERSION ${COLOR_VERSION} LANGUAGES CXX)

#############################################
#         ---- USEFUL VARIABLES ----
# OPENEXR_HOME - Root of ILMBase installation

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(IlmBase 2.2 REQUIRED)

add_library(color SHARED
  color/cmf.cpp
  color/color_space_rgb.cpp
  color/illuminant.cpp
  color/color_checker.cpp
  color/spd_conversion.cpp
  )

target_include_directories(color PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(color PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty)
target_include_directories(color PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/spdlog/include)
target_link_libraries(color PUBLIC IlmBase::Imath)
set_target_properties(color PROPERTIES VERSION ${COLOR_VERSION} SOVERSION ${COLOR_VERSION_MAJOR}.${COLOR_VERSION_MINOR})

add_executable(test_color test/test_color.cpp)
target_link_libraries(test_color color)

enable_testing()
add_test(test_color test_color)

export(TARGETS color FILE ColorConfig.cmake)

