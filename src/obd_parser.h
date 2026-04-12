#ifndef OBD_PARSER_H
#define OBD_PARSER_H

#include <string>
#include <vector>

enum class DriveStyle { SLOW = 0, NORMAL = 1, AGGRESSIVE = 2 };

struct OBDRecord {
    double speed;
    double rpm;
    double throttle;
    double coolant;
    double fuel;
    double intake;
    DriveStyle label;
};

class OBDParser {
public:
    int load(const std::string& filename);
    OBDRecord getRecord(int index) const;
    static int styleToInt(const std::string& label);

private:
    std::vector<OBDRecord> records;
};

#endif