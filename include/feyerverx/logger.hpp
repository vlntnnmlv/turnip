// Copyright 2025 Valentin Namleev

#pragma once

#include <format>
#include <print>
#include <string>

namespace feyerverx {
enum class LogLevel { Debug, Info, Warning, Error };

class Logger {
public:
    static Logger &instance();

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;

    template <typename... Args>
    void log(const LogLevel logLevel, const std::string_view format, Args &&...args) const {
        std::string message = std::vformat(format, std::make_format_args(args)...);
        std::string line = std::format("[{}] {}", logLevelToString(logLevel), message);
        std::print("{}\n", line);
    }

    void log(LogLevel logLevel, std::string_view message) const;

private:
    Logger();

    [[nodiscard]] std::string_view logLevelToString(LogLevel logLevel) const;
};
} // namespace feyerverx
