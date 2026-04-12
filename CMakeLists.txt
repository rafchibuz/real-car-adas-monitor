cmake_minimum_required(VERSION 3.20)
project(RealCarMonitor)

set(CMAKE_CXX_STANDARD 17) # Стандарт C++17 [cite: 94]

# Поиск OpenCV [cite: 95]
set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Настройка ONNX Runtime [cite: 96]
set(ONNXRUNTIME_ROOT "C:/onnxruntime")
include_directories(${ONNXRUNTIME_ROOT}/include)
link_directories(${ONNXRUNTIME_ROOT}/lib)

# Собираем все файлы из src [cite: 97]
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(RealCarMonitor ${SOURCES})
target_link_libraries(RealCarMonitor ${OpenCV_LIBS} onnxruntime)