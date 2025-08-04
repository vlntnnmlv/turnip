// Copyright 2025 Valentin Namleev

#include "./engine.hpp"

int main() {
  turnip::Engine engine = turnip::Engine(640, 480, "Turnip");
  engine.Run();

  return 0;
}
