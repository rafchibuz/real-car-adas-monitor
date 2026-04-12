#include <iostream>
#include <iomanip>
#include "obd_parser.h"

int main() {
    OBDParser parser("../data/dataset.csv");
    int count = parser.load();
    if (count <= 0) return 1;

    std::cout << std::left << std::setw(12) << "speed_kmh" << std::setw(12) << "engine_rpm" 
              << std::setw(15) << "throttle_pos" << std::setw(15) << "coolant_temp" 
              << std::setw(12) << "fuel_level" << std::setw(18) << "intake_air_temp" 
              << "label" << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    for (int i = 0; i < 5; ++i) {
        OBDRecord r = parser.getRecord(i);
        std::cout << std::left << std::setw(12) << r.speed_kmh << std::setw(12) << r.engine_rpm 
                  << std::setw(15) << r.throttle_pos << std::setw(15) << r.coolant_temp 
                  << std::setw(12) << r.fuel_level << std::setw(18) << r.intake_air_temp 
                  << r.label << std::endl;
    }
    return 0;
}