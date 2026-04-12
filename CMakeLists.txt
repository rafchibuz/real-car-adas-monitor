cmake_minimum_required(VERSION 3.20)
project(RealCarMonitor)

set(CMAKE_CXX_STANDARD 17)

# Игнорируем SSL для загрузки GTest
set(CMAKE_TLS_VERIFY OFF)

# --- БЛОК ТЕСТОВ (Google Test) ---
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- ПУТИ К ЗАГОЛОВКАМ (ВАЖНО!) ---
# Это заставит компилятор искать файлы в папке src
include_directories(src)

# --- ИСПОЛНЯЕМЫЕ ФАЙЛЫ ---

# Основное приложение
add_executable(RealCarMonitor 
    src/main.cpp 
    src/obd_parser.cpp
)

# Исполняемый файл тестов
add_executable(obd_tests 
    tests/test_obd_parser.cpp 
    src/obd_parser.cpp
)

# Указываем тестам, что они зависят от папки src и библиотеки GTest
target_include_directories(obd_tests PRIVATE src)
target_link_libraries(obd_tests GTest::gtest_main)

add_test(NAME OBDParserTests COMMAND obd_tests)