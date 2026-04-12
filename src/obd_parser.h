#ifndef OBD_PARSER_H
#define OBD_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

struct OBDRecord {
    std::string timestamp;
    double speed_kmh;
    double engine_rpm;
    double throttle_pos;
    double coolant_temp;
    double fuel_level;
    double intake_air_temp;
    std::string label; // SLOW, NORMAL, AGGRESSIVE
};

class OBDParser {
public:
    explicit OBDParser(const std::string& filename);
    int load(); 
    OBDRecord getRecord(int index) const;
    int getCount() const { return _records.size(); }

private:
    std::string _filename;
    std::vector<OBDRecord> _records;
    
    // Метод для определения стиля вождения по RPM и Speed
    std::string determineLabel(double rpm, double speed);
};

#endif