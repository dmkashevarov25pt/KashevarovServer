#include "Server.h"
#include "Logger.h"
#include "Errors.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

static void printHelp() {
    std::cout <<
        "Сервер вычислений векторов vcalc\n"
        "Использование: vcalc [ПАРАМЕТРЫ]\n\n"
        "Параметры:\n"
        "  -h, --help             Показать эту справку и завершить работу\n"
        "  -p, --port N           Порт прослушивания (по умолчанию: 33333)\n"
        "  -d, --database PATH    Файл базы пользователей (по умолчанию: /etc/vcalc.conf)\n"
        "  -l, --log PATH         Файл журнала (по умолчанию: /var/log/vcalc.log)\n\n"
        "Формат базы: <логин>:<пароль>\n";
}

int main(int argc, char** argv) {
    try {
        po::options_description desc("Параметры");
        desc.add_options()
            ("help,h", "Показать справку и завершить работу")
            ("port,p", po::value<unsigned short>()->default_value(33333), "Порт прослушивания")
            ("database,d", po::value<std::string>()->default_value("/etc/vcalc.conf"), "Путь к базе пользователей")
            ("log,l", po::value<std::string>()->default_value("/var/log/vcalc.log"), "Путь к файлу журнала");

        if (argc == 1) {
            printHelp();
            return 0;
        }

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            printHelp();
            return 0;
        }
        po::notify(vm);

        const auto port = vm["port"].as<unsigned short>();
        const auto db = vm["database"].as<std::string>();
        const auto log = vm["log"].as<std::string>();

        Server server(port, db, log);
        server.run();
    } catch (const po::error& e) {
        std::cerr << "Ошибка параметров командной строки: " << e.what() << '\n'
                  << "Запусти программу с -h для получения справки.\n";
        Logger::instance().init("/var/log/vcalc.log");
        Logger::instance().critical(e.what());
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << '\n';
        Logger::instance().critical(e.what());
        return 1;
    }
    return 0;
}
