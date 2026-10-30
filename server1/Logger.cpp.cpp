#include "Logger.h"
#include <ctime>
#include <iomanip>

Logger& Logger::instance() {
    static Logger inst;
    return inst;
}

void Logger::init(const std::string& path) {
    std::lock_guard<std::mutex> lk(mtx_);
    out_.open(path, std::ios::app);
    ready_ = out_.is_open();
    if (ready_) {
        info("=== Сервер запущен, файл журнала: " + path + " ===");
    }
}

void Logger::log(Severity sev, const std::string& msg) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (!ready_) return;

    std::time_t t = std::time(nullptr);
    std::tm tm{};
    localtime_r(&t, &tm);

    const char* sev_s = (sev == Severity::CRITICAL) ? "КРИТИЧЕСКАЯ" :
                        (sev == Severity::WARNING)  ? "ПРЕДУПРЕЖДЕНИЕ"  : "ИНФО";

    out_ << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << sev_s << "] " << msg << '\n';
    out_.flush();
}