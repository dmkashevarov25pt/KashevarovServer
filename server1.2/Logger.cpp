#include "Logger.h"
#include <ctime>
#include <iomanip>

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::init(const std::string& path) {
    bool initialized = false;
    {
        std::lock_guard<std::mutex> lk(mtx_);
        out_.open(path, std::ios::app);
        ready_ = out_.is_open();
        initialized = ready_;
    }
    if (initialized) {
        info("=== Server started; log file: " + path + " ===");
    }
}

void Logger::log(Severity sev, const std::string& msg) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (!ready_) return;

    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_r(&t, &tm);

    const char* sev_s = (sev == Severity::CRITICAL) ? "CRITICAL" :
                        (sev == Severity::WARNING)  ? "WARNING"  : "INFO";

    out_ << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << sev_s << "] " << msg << '\n';
    out_.flush();
}
