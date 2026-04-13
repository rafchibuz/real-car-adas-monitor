#include <cmath>
#include <iostream>
#include <string>
#include "dashboard.h"

// Если компилятор все еще не видит M_PI, определяем его сами
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void Dashboard::draw(cv::Mat& frame, const DashboardData& data) {
    // 1. Рисуем полупрозрачную подложку (черный прямоугольник с альфа-каналом)
    cv::Mat overlay = frame.clone();
    cv::rectangle(overlay, cv::Rect(0, 0, 320, 480), cv::Scalar(0, 0, 0), -1);
    cv::addWeighted(overlay, 0.6, frame, 0.4, 0, frame);

    // 2. Спидометр и Тахометр
    // Скорость краснеет после 90 (как в задании 5.4)
    cv::Scalar speedColor = (data.speed > 90) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
    drawGauge(frame, cv::Point(80, 100), 60, data.speed, 140, "SPEED", "km/h", speedColor);
    
    // Обороты краснеют после 4500
    cv::Scalar rpmColor = (data.rpm > 4500) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
    drawGauge(frame, cv::Point(240, 100), 60, data.rpm, 6000, "RPM", "", rpmColor);

    // 3. Линейные индикаторы (Температура, Топливо, Дроссель)
    drawLinearGauge(frame, cv::Rect(20, 200, 280, 20), data.temp, 120, "TEMP", 
                    (data.temp > 100 ? cv::Scalar(0, 0, 255) : cv::Scalar(255, 100, 0)));
    
    drawLinearGauge(frame, cv::Rect(20, 250, 280, 20), data.fuel, 100, "FUEL", 
                    (data.fuel < 15 ? cv::Scalar(0, 0, 255) : cv::Scalar(255, 200, 0)));
    
    drawLinearGauge(frame, cv::Rect(20, 300, 280, 20), data.throttle, 100, "THROTTLE", cv::Scalar(0, 255, 255));

    // 4. Стиль вождения (берется из классификатора)
    std::string styleText;
    cv::Scalar styleColor;
    if (data.drive_style == 0) { styleText = "SAFE"; styleColor = cv::Scalar(0, 255, 0); }
    else if (data.drive_style == 1) { styleText = "NORMAL"; styleColor = cv::Scalar(255, 255, 0); }
    else { styleText = "AGGRESSIVE"; styleColor = cv::Scalar(0, 0, 255); }

    cv::putText(frame, "STYLE: " + styleText, cv::Point(20, 380), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, styleColor, 2);

    // 5. Критические предупреждения
    if (data.temp > 100)
        cv::putText(frame, "WARNING: OVERHEAT!", cv::Point(20, 420), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
    if (data.fuel < 15)
        cv::putText(frame, "WARNING: LOW FUEL!", cv::Point(20, 450), cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 2);
}

void Dashboard::drawGauge(cv::Mat& frame, cv::Point center, int radius, float value, float max_value, 
                          const std::string& title, const std::string& unit, cv::Scalar color) {
    // Основной круг шкалы
    cv::circle(frame, center, radius, cv::Scalar(180, 180, 180), 2);
    
    // Вычисление угла стрелки (диапазон 240 градусов)
    float angle = 150 + (std::min(value, max_value) / max_value) * 240;
    float rad = angle * M_PI / 180.0;
    
    cv::Point endPoint(
        center.x + (int)((radius - 10) * cos(rad)),
        center.y + (int)((radius - 10) * sin(rad))
    );
    
    // Рисуем стрелку
    cv::line(frame, center, endPoint, color, 3);
    
    // Подписи данных
    cv::putText(frame, std::to_string((int)value) + unit, cv::Point(center.x - 25, center.y + 20), 
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
    cv::putText(frame, title, cv::Point(center.x - 30, center.y - radius - 10), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
}

void Dashboard::drawLinearGauge(cv::Mat& frame, cv::Rect rect, float value, float max_value, 
                                const std::string& title, cv::Scalar color) {
    // Контур шкалы
    cv::rectangle(frame, rect, cv::Scalar(100, 100, 100), 1);
    
    // Заполнение шкалы
    float fillWidth = (std::min(value, max_value) / max_value) * rect.width;
    cv::rectangle(frame, cv::Rect(rect.x, rect.y, (int)fillWidth, rect.height), color, -1);
    
    // Название индикатора
    cv::putText(frame, title, cv::Point(rect.x, rect.y - 5), 
                cv::FONT_HERSHEY_SIMPLEX, 0.4, cv::Scalar(255, 255, 255), 1);
}