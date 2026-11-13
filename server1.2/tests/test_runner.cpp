#include "test_support.h"
#include "1.1_Vector.cpp"
#include "1.2_Vector.cpp"
#include "1.3_Vector.cpp"
#include "1.4_Vector.cpp"
#include "1.5_Vector.cpp"
#include "2.1_HashProc.cpp"
#include "2.2_HashProc.cpp"
#include "2.3_HashProc.cpp"
#include "2.4_HashProc.cpp"
#include "2.5_HashProc.cpp"
#include "2.6_HashProc.cpp"
#include "3.1_Logger.cpp"
#include "3.2_Logger.cpp"
#include "4.1_Communicator.cpp"
#include "4.2_Communicator.cpp"
#include "4.3_Communicator.cpp"
#include "4.4_Communicator.cpp"
#include "5.1_Cli.cpp"
#include "5.2_Cli.cpp"
#include <iostream>
#include <utility>
#include <vector>

void test_1_1(); void test_1_2(); void test_1_3(); void test_1_4(); void test_1_5();
void test_2_1(); void test_2_2(); void test_2_3(); void test_2_4(); void test_2_5(); void test_2_6();
void test_3_1(); void test_3_2();
void test_4_1(); void test_4_2(); void test_4_3(); void test_4_4();
void test_5_1(); void test_5_2();

int main() {
    const std::vector<std::pair<const char*, void(*)()>> tests{
        {"1.1 Среднее арифметическое", test_1_1}, {"1.2 Пустой вектор", test_1_2},
        {"1.3 Один элемент", test_1_3}, {"1.4 Большие положительные значения", test_1_4},
        {"1.5 Большие отрицательные значения", test_1_5},
        {"2.1 Несуществующая база", test_2_1}, {"2.2 Пустая база", test_2_2},
        {"2.3 Формат хеша SHA-224 с солью", test_2_3}, {"2.4 Строка базы с CRLF", test_2_4},
        {"2.5 Разные пароли дают разные хеши", test_2_5},
        {"2.6 Неизвестный пользователь", test_2_6},
        {"3.1 Недоступный путь журнала", test_3_1}, {"3.2 Запись сообщений в журнал", test_3_2},
        {"4.1 Успешная аутентификация", test_4_1},
        {"4.2 Отказ для неизвестного логина", test_4_2},
        {"4.3 Обработка бинарного пакета с вектором", test_4_3},
        {"4.4 Отказ для неправильного пароля", test_4_4},
        {"5.1 Справка без параметров", test_5_1},
        {"5.2 Справка с параметром -h", test_5_2},
    };

    size_t failures = 0;
    for (const auto& [name, run] : tests) {
        try {
            run();
            std::cout << "[ПРОЙДЕН] " << name << '\n';
        } catch (const std::exception& e) {
            ++failures;
            std::cerr << "[ОШИБКА] " << name << ": " << e.what() << '\n';
        }
    }
    std::cout << tests.size() - failures << '/' << tests.size() << " тестов пройдено\n";
    return failures == 0 ? 0 : 1;
}
