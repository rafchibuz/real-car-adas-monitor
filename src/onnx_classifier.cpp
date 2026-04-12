#include "onnx_classifier.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>

ONNXClassifier::ONNXClassifier(const std::string& model_path, const std::string& json_path) {
    loadNormalizationParams(json_path);
        
#ifdef _WIN32
    std::wstring w_model_path(model_path.begin(), model_path.end());
    session = std::make_unique<Ort::Session>(env, w_model_path.c_str(), Ort::SessionOptions{nullptr});
#else
    session = std::make_unique<Ort::Session>(env, model_path.c_str(), Ort::SessionOptions{nullptr});
#endif
    if (!session) throw std::runtime_error("Failed to load ONNX model");
}

void ONNXClassifier::loadNormalizationParams(const std::string& json_path) {
    std::ifstream f(json_path);
    if (!f.is_open()) throw std::runtime_error("Could not open JSON: " + json_path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    std::string content = buffer.str();
    means = parseJsonArray(content, "\"mean\"");
    stds = parseJsonArray(content, "\"std\"");
}

std::vector<float> ONNXClassifier::parseJsonArray(const std::string& content, const std::string& key) {
    std::vector<float> result;
    size_t pos = content.find(key);
    if (pos == std::string::npos) return result;
    size_t start = content.find("[", pos) + 1;
    size_t end = content.find("]", start);
    std::string array_str = content.substr(start, end - start);
    std::stringstream ss(array_str);
    std::string val;
    while (std::getline(ss, val, ',')) { result.push_back(std::stof(val)); }
    return result;
}

ClassificationResult ONNXClassifier::classify(const std::vector<float>& features) {
    std::vector<float> normalized(6);
    for (int i = 0; i < 6; ++i) {
        normalized[i] = (features[i] - means[i]) / stds[i];
    }

    std::vector<int64_t> input_shape = {1, 6};
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        memory_info, normalized.data(), normalized.size(), input_shape.data(), input_shape.size()
    );

    // --- АВТОМАТИЧЕСКОЕ ОПРЕДЕЛЕНИЕ ИМЕН ---
    Ort::AllocatorWithDefaultOptions allocator;
    
    // Получаем реальное имя входа (Input 0)
    auto input_name_ptr = session->GetInputNameAllocated(0, allocator);
    const char* input_names[] = { input_name_ptr.get() };

    // Получаем реальное имя выхода (Output 0)
    auto output_name_ptr = session->GetOutputNameAllocated(0, allocator);
    const char* output_names[] = { output_name_ptr.get() };

    // Запуск
    auto output_tensors = session->Run(
        Ort::RunOptions{nullptr}, 
        input_names, &input_tensor, 1, 
        output_names, 1
    );

    float* logits = output_tensors[0].GetTensorMutableData<float>();

    std::vector<float> probs(3);
    float max_logit = *std::max_element(logits, logits + 3);
    float sum = 0.0f;
    for (int i = 0; i < 3; ++i) {
        probs[i] = std::exp(logits[i] - max_logit);
        sum += probs[i];
    }
    for (int i = 0; i < 3; ++i) probs[i] /= sum;

    int best_label = std::max_element(probs.begin(), probs.end()) - probs.begin();
    return {best_label, probs[best_label], probs};
}