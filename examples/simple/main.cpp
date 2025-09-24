#include "turnip.hpp"
#include <print>

int main() {
    try {
        turnip::Turnip app("Turnip", 800, 600);
        app.Run();
    } catch (std::runtime_error _e) {
        std::println("{}", _e.what());
    }
    return 0;
}