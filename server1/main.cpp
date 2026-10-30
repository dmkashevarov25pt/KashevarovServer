#include "Server.h"
#include "Logger.h"
#include "Errors.h"

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

static void printHelp() {
    std::cout <<
R"(Сервер вычислений vcalc
Использование:
  vcalc [ОПЦИИ]

Опции:
  -h, --help            Показать эту справку
  -p, --port N          Порт прослушивания (по умолчанию 33333)
  -d, --database PATH   Файл базы пользователей (по умолчанию /etc/vcalc.conf)
  -l, --log PATH        Файл журнала (по умолчанию /var/log/vcalc.log)

Формат базы:
  логин:пароль
)";
}

int main(int argc, char** argv) {
    try {
        po::options_description desc("Доступные опции");
        desc.add_options()
            ("help,h",     "показать справку")
            ("port,p",     po::value<unsigned short>()->default_value(33333), "порт прослушивания")
            ("database,d", po::value<std::string>()->default_value("/etc/vcalc.conf"), "файл базы данных")
            ("log,l",      po::value<std::string>()->default_value("/var/log/vcalc.log"), "файл журнала");

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

        unsigned short port = vm["port"].as<unsigned short>();
        std::string    db   = vm["database"].as<std::string>();
        std::string    log  = vm["log"].as<std::string>();

        Server srv(port, db, log);
        srv.run();

    } catch (const po::error& e) {
        std::cerr << "Ошибка параметров: " << e.what() << "\n";
        std::cerr << "Запустите с -h для справки.\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << "\n";
        Logger::instance().critical(e.what());
        return 1;
    }
    return 0;
}