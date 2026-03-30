cmake_minimum_required(VERSION 3.20)
project(RealCarADASMonitor VERSION 1.0)

# 1. Указываем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем проверку SSL для загрузки Google Test (чтобы не было ошибок сети)
set(CMAKE_TLS_VERIFY OFF)

# 2. Находим OpenCV (MinGW сборка)
# Указываем путь к твоей новой распакованной папке для MinGW
set(OpenCV_DIR "C:/opencv/build/x64/mingw/lib")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 3. Находим ONNX Runtime
set(ONNXRUNTIME_ROOT "C:/onnxruntime")
include_directories(${ONNXRUNTIME_ROOT}/include)
link_directories(${ONNXRUNTIME_ROOT}/lib)

# 4. Объявляем исполняемый файл RealCarMonitor
add_executable(RealCarMonitor 
    src/main.cpp 
    src/obd_parser.cpp
)

# Привязываем библиотеки к основному приложению
target_link_libraries(RealCarMonitor PRIVATE 
    ${OpenCV_LIBS} 
    "${ONNXRUNTIME_ROOT}/lib/onnxruntime.lib"
)

# 5. Подключаем Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP NEW
)
# Настройка для корректной сборки GTest на Windows с MinGW
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 6. Объявляем исполняемый файл RunTests для тестов
add_executable(RunTests 
    tests/test_obd_parser.cpp 
    src/obd_parser.cpp
)
target_link_libraries(RunTests PRIVATE gtest_main)