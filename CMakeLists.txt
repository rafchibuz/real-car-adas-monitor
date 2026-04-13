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

# --- НАСТРОЙКА БИБЛИОТЕК ---

# 1. OpenCV (Важно: путь к папке с OpenCVConfig.cmake)
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)

# 2. ONNX Runtime
set(ONNXRUNTIME_DIR "C:/onnxruntime")

# --- ПУТИ К ЗАГОЛОВКАМ (Headers) ---
include_directories(
    src 
    "${ONNXRUNTIME_DIR}/include"
    ${OpenCV_INCLUDE_DIRS}
)

# --- ИСПОЛНЯЕМЫЕ ФАЙЛЫ ---

# Основное приложение
add_executable(RealCarMonitor 
    src/main.cpp 
    src/obd_parser.cpp
    src/onnx_classifier.cpp
    src/dashboard.cpp
)

# Линковка библиотек
# Для MSVC лучше указывать полный путь к .lib файлу ONNX
target_link_libraries(RealCarMonitor PRIVATE 
    "${ONNXRUNTIME_DIR}/lib/onnxruntime.lib"
    ${OpenCV_LIBRARIES}
)

# --- ТЕСТЫ ---
add_executable(obd_tests 
    tests/test_obd_parser.cpp 
    src/obd_parser.cpp
)
target_include_directories(obd_tests PRIVATE src)
target_link_libraries(obd_tests GTest::gtest_main)

add_test(NAME OBDParserTests COMMAND obd_tests)