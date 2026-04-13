#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
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

         // --- БЛОК ВЕРИФИКАЦИИ СО СЛУЧАЙНЫМИ ДАННЫМИ (Задание 4.5) ---
        std::cout << "\n" << std::string(55, '=') << std::endl;
        std::cout << "       ONNX MODEL VERIFICATION (20 RANDOM RECORDS)" << std::endl;
        std::cout << std::string(55, '=') << std::endl;
        
        std::cout << std::left << std::setw(6) << "ID" 
                  << std::setw(15) << "True Label" 
                  << std::setw(15) << "Prediction" 
                  << std::setw(10) << "Conf (%)" << std::endl;
        std::cout << std::string(55, '-') << std::endl;

        int correct_count = 0;
        int test_limit = 20;
        int total_records = parser.getRecordCount();

        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        for (int i = 0; i < test_limit; ++i) {
            // Выбираем случайный индекс от 0 до total_records - 1
            int random_idx = std::rand() % total_records;
            
            auto rec = parser.getRecord(random_idx);
            std::vector<float> features = {
                (float)rec.speed, (float)rec.rpm, (float)rec.throttle, 
                (float)rec.coolant, (float)rec.fuel, (float)rec.intake
            };

            auto result = classifier.classify(features);
            int true_val = static_cast<int>(rec.label);

            if (true_val == result.label) correct_count++;

            std::cout << std::left << std::setw(6) << random_idx 
                      << std::setw(15) << true_val 
                      << std::setw(15) << result.label 
                      << std::fixed << std::setprecision(1) << (result.confidence * 100.0f) << "%" 
                      << (true_val == result.label ? " [OK]" : " [ERR]") 
                      << std::endl;
        }

        std::cout << std::string(55, '-') << std::endl;
        float accuracy = (float)correct_count / test_limit * 100.0f;
        std::cout << "TOTAL ACCURACY (RANDOM 20): " << accuracy << "%" << std::endl;
        std::cout << std::string(55, '=') << std::endl;

        // Ожидание подтверждения запуска
        std::string input;
        std::cout << "\nReady to launch dashboard. Press 'Y' to continue: ";
        while (true) {
            std::getline(std::cin, input);
            if (input == "Y" || input == "y") break;
            std::cout << "Waiting for 'Y' to start... ";
        }

        // --- ОСНОВНОЙ ЦИКЛ ВИЗУАЛИЗАЦИИ ---
        cv::Mat background = cv::Mat::zeros(480, 640, CV_8UC3);
        std::cout << "\nStarting Dashboard. Press ESC in window to exit." << std::endl;

        for (int i = 0; i < parser.getRecordCount(); ++i) {
            auto rec = parser.getRecord(i);

            std::vector<float> features = {
                (float)rec.speed, (float)rec.rpm, (float)rec.throttle, 
                (float)rec.coolant, (float)rec.fuel, (float)rec.intake
            };

            auto result = classifier.classify(features);

            DashboardData uiData;
            uiData.speed = (float)rec.speed;
            uiData.rpm = (float)rec.rpm;
            uiData.temp = (float)rec.coolant;
            uiData.fuel = (float)rec.fuel;
            uiData.throttle = (float)rec.throttle;
            uiData.drive_style = result.label;

            cv::Mat frame = background.clone();
            db.draw(frame, uiData);

            std::cout << "Processing Record #" << i << "\r" << std::flush;

            cv::imshow("Real-Car ADAS Monitor", frame);
            if (cv::waitKey(100) == 27) break; 
        }

    } catch (const std::exception& e) {
        std::cerr << "\nCRITICAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}