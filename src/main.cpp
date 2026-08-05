#include <cstdlib>
#include <iostream>

#include "triangleApplication.hpp"

int main(void) {
  TriangleApplication app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return (0);
}
