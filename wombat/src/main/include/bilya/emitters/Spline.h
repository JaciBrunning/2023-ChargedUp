#pragma once

#include "bilya/emitters/BaseEmitter.h"

#include <optional>

namespace bilya {
  template<size_t N>
  struct SplineControlPoint {
    Vec<N> position;
    Vec<N> derivative;
  };

  template<size_t N>
  struct SplinePoint {
    Vec<N> position;
    Vec<N> lookaheadPosition;
  };

  template<size_t N>
  struct SplineConfig {
    std::vector<SplineControlPoint<N>> controlPoints;
    double lookahead = 0;
    double period = 0.05;
  };

  template<size_t N>
  class Spline : public Emitter<N> {
   public:
    Spline(SplineConfig<N> config, EmitterConfig emitter) : Emitter<N>(emitter), _config(config) {
      // Calculate Spline
      CalculateFullSpline();
    }

    std::pair<Vec<N>, bool> CalculateSplinePointWithTrim(double t) {
      size_t i0 = (size_t) std::floor(t);
      size_t i1 = i0 + 1;

      if (i1 >= _config.controlPoints.size()) {
        // We're past the end of the spline, return the last point on the spline and 
        // say we're trimmed
        return {
          (_config.controlPoints.cend() - 1)->position,
          true
        };
      } else {
        t = t - i0;

        SplineControlPoint<N> control0 = _config.controlPoints[i0];
        SplineControlPoint<N> control1 = _config.controlPoints[i1];

        double t2 = t*t;
        double t3 = t*t*t;

        // Calculate the point on the cubic hermite spline
        return {
          (2*t3 - 3*t2 + 1)*control0.position + (t3 - 2*t2 + t)*control0.derivative + (-2*t3 + 3*t2)*control1.position + (t3 - t2)*control1.derivative,
          false
        };
      }
    }

    void CalculateFullSpline() {
      double t = 0;
      while (true) {
        auto p = CalculateSplinePointWithTrim(t);
        auto lookahead = CalculateSplinePointWithTrim(t + _config.lookahead);

        if (!p.second) {
          // Not trimmed
          _fullSpline.emplace_back(SplinePoint<N> {
            p.first, lookahead.first
          });
        } else {
          // Trimmed
          break;
        }
        t += _config.period;
      }
    }

    const std::vector<SplinePoint<N>> GetSpline() const {
      return _fullSpline;
    }

    PointGradient<N> Calculate(const Vec<N> &position) const override {
      std::optional<PointGradient<N>> closest = {};
      auto closestDist = -1;

      for (size_t i = 0; i < _fullSpline.size(); i++) {
        auto result = this->CalculateEmitter(_fullSpline[i].lookaheadPosition, position);
        auto dist = (_fullSpline[i].position - position).norm();

        if (!closest.has_value() || dist < closestDist) {
          closest = result;
          closestDist = dist;
        }
      }

      return closest.value();
    }
   private:
    SplineConfig<N> _config;
    std::vector<SplinePoint<N>> _fullSpline;
  };
}