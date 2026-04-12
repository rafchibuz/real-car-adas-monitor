#include <gtest/gtest.h>
#include "../src/obd_parser.h"
#include <fstream>

// 1. Тест: Ошибка при отсутствии файла
TEST(OBDParserTest, FileNotFound) {
    OBDParser parser("missing.csv");
    EXPECT_EQ(parser.load(), -1);
}

// 2. Тест: Корректная загрузка данных
TEST(OBDParserTest, LoadSuccess) {
    std::ofstream f("test.csv");
    f << "t,rpm,speed,thr,v,v,v,cool,int,v,v,v,fuel\n0,2000,50,15,0,0,0,90,40,0,0,0,50\n";
    f.close();
    OBDParser parser("test.csv");
    EXPECT_GT(parser.load(), 0);
}

// 3. Тест: Проверка лейбла SLOW
TEST(OBDParserTest, LabelSlow) {
    OBDParser parser("test.csv");
    parser.load();
    EXPECT_EQ(parser.getRecord(0).label, "SLOW");
}

// 4. Тест: Проверка Out of Range
TEST(OBDParserTest, OutOfRange) {
    OBDParser parser("test.csv");
    parser.load();
    EXPECT_THROW(parser.getRecord(100), std::out_of_range);
}

// 5. Тест: Проверка корректности колонок (Coolant)
TEST(OBDParserTest, DataIntegrity) {
    OBDParser parser("test.csv");
    parser.load();
    EXPECT_DOUBLE_EQ(parser.getRecord(0).coolant_temp, 90.0);
}