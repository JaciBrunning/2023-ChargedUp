#pragma once

#include "bilya/Types.h"

namespace bilya {
  struct EmitterConfig {
    double alpha;
    double gamma;
  };

  class Emitter {
   public:
    Emitter(const EmitterConfig &config);
    virtual const EmitterConfig &GetConfig() const;
    virtual PointGradient Calculate(const Position2d &position) const = 0;

    PointGradient CalculateEmitter(const Position2d &emit_src, const Position2d &position) const;
   private:
    EmitterConfig _config;
  };

  template<typename It>
  PointGradient CalculateNet(const Position2d &position, It start, It end) {
    PointGradient cumulative;

    for (auto it = start; it != end; it++) {
      cumulative = cumulative + (*it)->Calculate(position);
    }

    cumulative.position = position;

    return cumulative;
  }
}