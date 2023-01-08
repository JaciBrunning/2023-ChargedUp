#pragma once

#include "bilya/Types.h"

namespace bilya {
  struct EmitterConfig {
    double alpha;
    double gamma;
  };

  template<size_t N>
  class Emitter {
   public:
    Emitter(const EmitterConfig &config) : _config(config) {}

    virtual const EmitterConfig &GetEmitterConfig() const {
      return _config;
    }

    virtual PointGradient<N> Calculate(const Vec<N> &position) const = 0;

    PointGradient<N> CalculateEmitter(const Vec<N> &emit_src, const Vec<N> &position) const {
      auto delta = emit_src - position;
      auto dist = delta.norm();

      auto config = GetEmitterConfig();

      double potential = -config.alpha * exp( -(config.gamma / 2 * dist * dist) );
      auto gradient = delta * -config.gamma * potential;

      return PointGradient<N> {
        position,
        gradient,
        potential
      };
    }
   private:
    EmitterConfig _config;
  };

  template<size_t N, typename It>
  PointGradient<N> CalculateNet(const Vec<N> &position, It start, It end) {
    PointGradient cumulative;

    for (auto it = start; it != end; it++) {
      cumulative = cumulative + (*it)->Calculate(position);
    }

    cumulative.position = position;

    return cumulative;
  }
}