cmake_minimum_required(VERSION 3.20)
project(RealCarMonitor)

set(CMAKE_CXX_STANDARD 17)

# Игнорируем SSL, если твоя система всё еще ругается на сертификаты
set(CMAKE_TLS_VERIFY OFF)

# --- БЛОК ТЕСТОВ ---
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Для Windows: предотвращаем конфликты рантаймов
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

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

target_link_libraries(obd_tests GTest::gtest_main)
add_test(NAME OBDParserTests COMMAND obd_tests)