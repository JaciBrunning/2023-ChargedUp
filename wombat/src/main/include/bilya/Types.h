#pragma once

#include <Eigen/Core>

namespace bilya {
  template<size_t N>
  using Vec = Eigen::Vector<double, N>;

  template<size_t N>
  struct PointGradient {
    Vec<N> position;
    Vec<N> gradient;
    double potential{0};

    PointGradient operator+(const PointGradient &other) {
      return PointGradient {
        position,
        gradient + other.gradient,
        potential + other.potential
      };
    }

    PointGradient operator-(const PointGradient &other) {
      return PointGradient {
        position,
        gradient - other.gradient,
        potential - other.potential
      };
    }
  };
}