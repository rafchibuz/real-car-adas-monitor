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

# --- ШАГ 1.3: НАСТРОЙКА ONNX RUNTIME (Путь C:/onnxruntime) ---
# Указываем корень, где ты распаковал либу
set(ONNXRUNTIME_DIR "C:/onnxruntime")

# Добавляем пути к заголовкам и библиотекам
include_directories(src "${ONNXRUNTIME_DIR}/include")
link_directories("${ONNXRUNTIME_DIR}/lib")

# --- ИСПОЛНЯЕМЫЕ ФАЙЛЫ ---

# Основное приложение (со всеми .cpp из src)
add_executable(RealCarMonitor 
    src/main.cpp 
    src/obd_parser.cpp
    src/onnx_classifier.cpp
)

# Линковка ONNX
target_link_libraries(RealCarMonitor PRIVATE onnxruntime)

# Тесты
add_executable(obd_tests 
    tests/test_obd_parser.cpp 
    src/obd_parser.cpp
)
target_include_directories(obd_tests PRIVATE src)
target_link_libraries(obd_tests GTest::gtest_main)

add_test(NAME OBDParserTests COMMAND obd_tests)