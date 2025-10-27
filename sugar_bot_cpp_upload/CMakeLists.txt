cmake_minimum_required(VERSION 3.16)
project(sugar_Bot LANGUAGES CXX)

# --- C++ standard  ---------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Warnings / strictness -------------------------------------
if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --- Library: sugar_core  ----------------
add_library(sugar_core
  src/csv.cpp
  src/utils.cpp
  src/series.cpp
  src/indicators_sma.cpp
  src/indicators_ema.cpp
  src/indicators_roc.cpp
  src/indicators_composite.cpp
  src/strategy_roc_sma.cpp
  src/backtester.cpp
  src/sweep.cpp
)

target_include_directories(sugar_core PUBLIC "${CMAKE_SOURCE_DIR}/include")

# --- Executable: sugar_Bot  -------------------------
add_executable(sugar_Bot app/main.cpp)
target_link_libraries(sugar_Bot PRIVATE sugar_core)

# --- Put build artifacts in ./out  ----------
# Single-config generators (Makefiles, Ninja):
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")

# Multi-config (Visual Studio, Xcode):
foreach(cfg IN LISTS CMAKE_CONFIGURATION_TYPES)
  string(TOUPPER "${cfg}" CFG_UP)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CFG_UP} "${CMAKE_SOURCE_DIR}/out")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CFG_UP} "${CMAKE_SOURCE_DIR}/out")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CFG_UP} "${CMAKE_SOURCE_DIR}/out")
endforeach()
