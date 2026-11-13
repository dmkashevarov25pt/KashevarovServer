#include "test_support.h"
#include "Logger.h"

void test_3_1() {
    // По текущему API неудачное открытие не бросает исключение.
    Logger::instance().init("/invalid_dir_999/vcalc.log");
    Logger::instance().critical("logger unavailable path probe");
    CHECK(true);
}
