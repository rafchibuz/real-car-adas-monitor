#include "obd_parser.h"
#include <sstream>
#include <iostream>

OBDParser::OBDParser(const std::string& filename) : _filename(filename) {}

std::string OBDParser::determineLabel(double rpm, double speed) {
    if (rpm > 4000 || speed > 110) return "AGGRESSIVE";
    if (rpm > 2500 || speed > 60) return "NORMAL";
    return "SLOW";
}

int OBDParser::load() {
    std::ifstream file(_filename);
    if (!file.is_open()) return -1;

    _records.clear();
    std::string line;
    std::getline(file, line); // Скип заголовка

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string val;
        std::vector<std::string> cols;

        while (std::getline(ss, val, ',')) cols.push_back(val);

        try {
            if (cols.size() < 13) throw std::runtime_error("Row too short");

            OBDRecord rec;
            rec.timestamp = cols[0];
            rec.engine_rpm = std::stod(cols[1]);
            rec.speed_kmh = std::stod(cols[2]);
            rec.throttle_pos = std::stod(cols[3]);
            rec.coolant_temp = std::stod(cols[7]);
            rec.fuel_level = std::stod(cols[12]);
            rec.intake_air_temp = std::stod(cols[8]);
            
            // Определяем стиль
            rec.label = determineLabel(rec.engine_rpm, rec.speed_kmh);
            
            _records.push_back(rec);
        } catch (...) {
            std::cerr << "Warning: Invalid row skipped" << std::endl;
        }
    }
    return _records.size();
}

OBDRecord OBDParser::getRecord(int index) const {
    if (index < 0 || index >= (int)_records.size()) throw std::out_of_range("Range error");
    return _records[index];
}