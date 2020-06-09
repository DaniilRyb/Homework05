cmake_minimum_required(VERSION 3.10)
project(banking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(BUILD_TESTS "Build tests" OFF)
add_library(account STATIC banking/Account.cpp)
target_include_directories(account
 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_library(transaction STATIC banking/Transaction.cpp)
target_include_directories(transaction
 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)
if(BUILD_TESTS)
  # Включить поддержку тестирования:
  enable_testing()
  add_subdirectory(third-party/gtest)
  # Создание списка файлов, соответствующих выражению и сохранение его в переменную
  file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
  target_compile_options(check PRIVATE --coverage)
  target_link_libraries(check PRIVATE account transaction gtest_main gmock_main  --coverage)
  # Добавление тестов к проекту
  add_test(NAME check COMMAND check)
endif()
