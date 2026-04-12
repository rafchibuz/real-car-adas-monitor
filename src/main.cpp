#include <iostream>
#include "obd_parser.h"

int main() {
    OBDParser parser;
    int count = parser.load("../data/obd_data.csv");

    if (count == -1) {
        std::cout << "File not found!" << std::endl;
        return 1;
    }

    std::cout << "Loaded " << count << " records." << std::endl;

    int s = 0, n = 0, a = 0;
    for (int i = 0; i < count; ++i) {
        auto rec = parser.getRecord(i);
        if (rec.label == DriveStyle::SLOW) s++;
        else if (rec.label == DriveStyle::NORMAL) n++;
        else a++;

        if (i < 5) {
            std::cout << "Record " << i << ": Speed=" << rec.speed << " RPM=" << rec.rpm << std::endl;
        }
    }

    std::cout << "\nStats:\nSLOW: " << s << "\nNORMAL: " << n << "\nAGGRESSIVE: " << a << std::endl;
    return 0;
}