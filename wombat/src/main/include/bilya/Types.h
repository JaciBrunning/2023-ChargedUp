#pragma once

#include <frc/geometry/Translation2d.h>
#include <units/velocity.h>

namespace bilya {
  using Position2d = frc::Translation2d;

  struct Gradient2d {
    units::meters_per_second_t x, y;

    Gradient2d operator+(const Gradient2d &other);
    Gradient2d operator-(const Gradient2d &other);
  };

  struct PointGradient {
    Position2d position{0_m, 0_m};
    Gradient2d gradient{0_mps, 0_mps};
    double potential{0};

    PointGradient operator+(const PointGradient &other);
    PointGradient operator-(const PointGradient &other);
  };
}