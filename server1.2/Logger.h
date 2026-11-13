#pragma once
#include <string>
#include <mutex>
#include <fstream>

enum class Severity { INFO, WARNING, CRITICAL };

/** Thread-safe append-only logger with timestamps and severity labels. */
class Logger {
public:
    /** Return the process-wide logger instance. */
    static Logger& instance();

    /** Open the log file in append mode. Logging stays disabled if opening fails. */
    void init(const std::string& path);

    /** Write a timestamped message with the selected severity. */
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
