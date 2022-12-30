#include "bilya/Types.h"
using namespace bilya;

Gradient2d Gradient2d::operator+(const Gradient2d &other) {
  return Gradient2d {
    x + other.x,
    y + other.y
  };
}

Gradient2d Gradient2d::operator-(const Gradient2d &other) {
  return Gradient2d {
    x - other.x,
    y - other.y
  };
}

PointGradient PointGradient::operator+(const PointGradient &other) {
  return PointGradient {
    position,
    gradient + other.gradient,
    potential + other.potential
  };
}

PointGradient PointGradient::operator-(const PointGradient &other) {
  return PointGradient {
    position,
    gradient - other.gradient,
    potential - other.potential
  };
}