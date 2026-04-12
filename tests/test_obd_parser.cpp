#include <gtest/gtest.h>
#include "obd_parser.h"
#include <fstream>

TEST(OBDParser, StyleConversion) {
    EXPECT_EQ(OBDParser::styleToInt("SLOW"), 0);
    EXPECT_EQ(OBDParser::styleToInt("NORMAL"), 1);
    EXPECT_EQ(OBDParser::styleToInt("AGGRESSIVE"), 2);
}

TEST(OBDParser, FileNotFound) {
    OBDParser parser;
    EXPECT_EQ(parser.load("missing.csv"), -1);
}

TEST(OBDParser, OutOfRange) {
    OBDParser parser;
    EXPECT_THROW(parser.getRecord(0), std::out_of_range);
}

TEST(OBDParser, CorrectParsing) {
    std::ofstream tmp("test.csv");
    tmp << "s,r,t,c,f,i,l\n10,1000,15,90,50,30,SLOW\n";
    tmp.close();

    OBDParser parser;
    EXPECT_EQ(parser.load("test.csv"), 1);
    EXPECT_EQ(parser.getRecord(0).speed, 10);
    EXPECT_EQ(parser.getRecord(0).label, DriveStyle::SLOW);
    std::remove("test.csv");
}

TEST(OBDParser, SkipInvalidRow) {
    std::ofstream tmp("test_bad.csv");
    tmp << "h\n10,1000,15,90,50,30,SLOW\nbad,data,here\n20,2000,30,90,50,30,NORMAL\n";
    tmp.close();

    OBDParser parser;
    EXPECT_EQ(parser.load("test_bad.csv"), 2); // Битая строка пропущена
    std::remove("test_bad.csv");
}