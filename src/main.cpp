#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/opencv.hpp>
#include "obd_parser.h"
#include "onnx_classifier.h"
#include "dashboard.h"

int main() {
    try {
        // 1. Инициализация компонентов
        OBDParser parser;
        if (parser.load("data/obd_data.csv") <= 0) {
            throw std::runtime_error("Dataset not found or empty!");
        }

        ONNXClassifier classifier("models/driver_classifier.onnx", "models/normalization_params.json");

        Dashboard db;
        
        // Создаем базовый черный фон (имитация камеры)
        cv::Mat background = cv::Mat::zeros(480, 640, CV_8UC3);

        std::cout << "Starting ADAS Real-Time Monitor Simulation..." << std::endl;
        std::cout << "Press ESC to exit." << std::endl;

        // 2. Главный цикл визуализации (проходим по всем записям из CSV)
        for (int i = 0; i < parser.getRecordCount(); ++i) {
            auto rec = parser.getRecord(i);

            // Подготовка данных для нейронки
            std::vector<float> features = {
                (float)rec.speed, (float)rec.rpm, (float)rec.throttle, 
                (float)rec.coolant, (float)rec.fuel, (float)rec.intake
            };

            // Классификация стиля вождения
            auto result = classifier.classify(features);

            // Заполнение структуры для отрисовки Dashboard
            DashboardData uiData;
            uiData.speed = (float)rec.speed;
            uiData.rpm = (float)rec.rpm;
            uiData.temp = (float)rec.coolant;
            uiData.fuel = (float)rec.fuel;
            uiData.throttle = (float)rec.throttle;
            uiData.drive_style = result.label; // 0=SLOW, 1=NORMAL, 2=AGGRESSIVE

            // Отрисовка
            cv::Mat frame = background.clone(); // Берем чистый кадр
            db.draw(frame, uiData);

            // Вывод информации в консоль для дебага
            std::cout << "Record #" << i << " | Speed: " << rec.speed 
                      << " | Style: " << result.label << " (" << (int)(result.confidence * 100) << "%)" << "\r" << std::flush;

            // Показ окна
            cv::imshow("Real-Car ADAS Monitor", frame);

            // Задержка 100мс (чтобы имитировать поток данных)
            if (cv::waitKey(100) == 27) break; 
        }

    } catch (const std::exception& e) {
        std::cerr << "\nCRITICAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}