// Copyright 2025 Valentin Namleev

#include "feyerverx/logger.hpp"

namespace feyerverx {
Logger &Logger::instance() {
    static Logger instance;

    return instance;
}

Logger::Logger() {}

void Logger::log(LogLevel logLevel, const std::string_view message) const {
    std::string line = std::format("[{}] {}", logLevelToString(logLevel), message);
    std::print("{}\n", line);
}

std::string_view Logger::logLevelToString(LogLevel logLevel) const {
    switch (logLevel) {
    case LogLevel::Debug:
        return "DEBUG";
    case LogLevel::Info:
        return "INFO";
    case LogLevel::Warning:
        return "WARN";
    case LogLevel::Error:
        return "ERROR";
    }
    return "?";
}
}