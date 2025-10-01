// Copyright 2025 Valentin Namleev

#include <print>

#include <feyerverx/error.hpp>
#include <feyerverx/fey.hpp>

int main() {
    try {
        feyerverx::Fey fey("Turnip", 800, 600);
        fey.run();
    } catch (feyerverx::FeyError &e) {
        std::print("{}", e.what());
    }
    return 0;
}