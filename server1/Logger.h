#pragma once
#include <string>
#include <mutex>
#include <fstream>

enum class Severity { INFO, WARNING, CRITICAL };

class Logger {
public:
    static Logger& instance();

    void init(const std::string& path);
    void log(Severity sev, const std::string& msg);

    void info(const std::string& m)     { log(Severity::INFO, m); }
    void warn(const std::string& m)     { log(Severity::WARNING, m); }
    void critical(const std::string& m) { log(Severity::CRITICAL, m); }

private:
    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::mutex    mtx_;
    std::ofstream out_;
    bool          ready_ = false;
};