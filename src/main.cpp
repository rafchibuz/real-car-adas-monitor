#include <iostream>
#include <iomanip>
#include <vector>
#include "obd_parser.h"
#include "onnx_classifier.h"

int main() {
    try {
        // --- ЧАСТЬ 1: Загрузка и прошлая статистика (Шаг 2.5) ---
        OBDParser parser;
        int count = parser.load("data/obd_data.csv");

        if (count <= 0) {
            std::cerr << "Error: Dataset not found or empty!" << std::endl;
            return 1;
        }

        std::cout << "=== OBD Data Statistics ===" << std::endl;
        std::cout << "Total records loaded: " << count << std::endl;

        int s = 0, n = 0, a = 0;
        for (int i = 0; i < count; ++i) {
            auto rec = parser.getRecord(i);
            if (rec.label == DriveStyle::SLOW) s++;
            else if (rec.label == DriveStyle::NORMAL) n++;
            else a++;
        }

        std::cout << "SLOW: " << s << " | NORMAL: " << n << " | AGGRESSIVE: " << a << std::endl;
        std::cout << "-------------------------------------------" << std::endl;

        // --- ЧАСТЬ 2: Классификация нейросетью (Шаг 4.5) ---
        ONNXClassifier classifier("C:/Users/rafchibus/real-car-adas-monitor/models/driver_classifier.onnx", 
                                "C:/Users/rafchibus/real-car-adas-monitor/models/normalization_params.json");

        std::cout << "\n=== Neural Network Classification (First 20) ===" << std::endl;
        std::cout << std::left << std::setw(6) << "#" 
                  << std::setw(12) << "True Label" 
                  << std::setw(12) << "Prediction" 
                  << std::setw(10) << "Conf" << std::endl;
        std::cout << std::string(40, '-') << std::endl;

        int correct = 0;
        int test_limit = (count < 20) ? count : 20;

        for (int i = 0; i < test_limit; ++i) {
            auto rec = parser.getRecord(i);
            
            // Подготовка признаков для нейронки
            std::vector<float> features = {
                (float)rec.speed, (float)rec.rpm, (float)rec.throttle, 
                (float)rec.coolant, (float)rec.fuel, (float)rec.intake
            };

            auto result = classifier.classify(features);
            int true_val = static_cast<int>(rec.label);

            if (true_val == result.label) correct++;

            std::cout << std::left << std::setw(6) << i 
                      << std::setw(12) << true_val 
                      << std::setw(12) << result.label 
                      << std::fixed << std::setprecision(2) << result.confidence 
                      << std::endl;
        }

        std::cout << std::string(40, '-') << std::endl;
        std::cout << "Model Accuracy on these samples: " << (float)correct / test_limit * 100 << "%" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "CRITICAL ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}