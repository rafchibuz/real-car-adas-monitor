#ifndef ONNX_CLASSIFIER_H
#define ONNX_CLASSIFIER_H

#include <vector>
#include <string>
#include <onnxruntime_cxx_api.h>

struct ClassificationResult {
    int label;
    float confidence;
    std::vector<float> scores;
};

class ONNXClassifier {
public:
    ONNXClassifier(const std::string& model_path, const std::string& json_path);
    ClassificationResult classify(const std::vector<float>& features);

private:
    // Шаг 4.4: Минимальный JSON парсер
    void loadNormalizationParams(const std::string& json_path);
    std::vector<float> parseJsonArray(const std::string& content, const std::string& key);

    // Данные для нормализации
    std::vector<float> means;
    std::vector<float> stds;

    // Среда и сессия ONNX
    Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "DriverClassifier"};
    std::unique_ptr<Ort::Session> session;
    
    // Описатели входов/выходов
    std::vector<const char*> input_names = {"features"};
    std::vector<const char*> output_names = {"class_scores"};
};

#endif