#include "obd_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

int OBDParser::styleToInt(const std::string& label) {
    if (label == "SLOW") return 0;
    if (label == "NORMAL") return 1;
    if (label == "AGGRESSIVE") return 2;
    return -1;
}

int OBDParser::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return -1;

    records.clear();
    std::string line, header;
    std::getline(file, header); // Пропускаем заголовок

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        std::vector<std::string> row;

        while (std::getline(ss, val, ',')) {
            row.push_back(val);
        }

        if (row.size() != 7) {
            std::cerr << "Skip invalid row: " << line << std::endl;
            continue;
        }

        try {
            OBDRecord rec;
            rec.speed = std::stod(row[0]);
            rec.rpm = std::stod(row[1]);
            rec.throttle = std::stod(row[2]);
            rec.coolant = std::stod(row[3]);
            rec.fuel = std::stod(row[4]);
            rec.intake = std::stod(row[5]);
            
            int s = styleToInt(row[6]);
            if (s == -1) throw std::runtime_error("Unknown style");
            rec.label = static_cast<DriveStyle>(s);

            records.push_back(rec);
        } catch (...) {
            std::cerr << "Error parsing row: " << line << std::endl;
        }
    }
    return static_cast<int>(records.size());
}

OBDRecord OBDParser::getRecord(int index) const {
    if (index < 0 || index >= static_cast<int>(records.size())) {
        throw std::out_of_range("Index out of bounds");
    }
    return records[index];
}