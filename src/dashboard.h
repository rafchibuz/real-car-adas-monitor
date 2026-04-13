#pragma once
#include <opencv2/opencv.hpp>
#include <string>

struct DashboardData {
    float speed;
    float rpm;
    float temp;
    float fuel;
    float throttle;
    int drive_style; // 0: SLOW, 1: NORMAL, 2: AGGRESSIVE
};

class Dashboard {
public:
    void draw(cv::Mat& frame, const DashboardData& data);

private:
    // Круговой прибор (Спидометр/Тахометр)
    void drawGauge(cv::Mat& frame, cv::Point center, int radius, float value, float max_value, 
                   const std::string& title, const std::string& unit, cv::Scalar color);
    
    // Линейный прибор (Полоски)
    void drawLinearGauge(cv::Mat& frame, cv::Rect rect, float value, float max_value, 
                         const std::string& title, cv::Scalar color);
};