#pragma once

#include "bilya/emitters/BaseEmitter.h"

namespace bilya {
  enum class WallSide {
    BOTH, POSITIVE, NEGATIVE
  };
  
  struct WallConfig {
    Vec<2> from, to;
    bool overflow = true;
    WallSide side = WallSide::BOTH;
  };

  class Wall : public Emitter<2> {
   public:
    Wall(WallConfig wallConfig, EmitterConfig emitterConfig) : Emitter<2>(emitterConfig), _wallConfig(wallConfig) {}

    PointGradient<2> Calculate(const Vec<2> &position) const override {
      auto v = _wallConfig.to - _wallConfig.from;
      auto u = _wallConfig.from - position;

      auto t = (-(v.dot(u)) / v.squaredNorm());
      auto d = (v[0] * u[1] - v[1] * u[0]);

      PointGradient<2> result;
      if ((_wallConfig.side == WallSide::POSITIVE && d < 0) || (_wallConfig.side == WallSide::NEGATIVE && d > 0))
        return result;
      
      if (t < 0) {
        if (_wallConfig.overflow)
          return this->CalculateEmitter(_wallConfig.from, position);
      } else if (t > 1) {
        if (_wallConfig.overflow)
          return this->CalculateEmitter(_wallConfig.to, position);
      } else {
        return this->CalculateEmitter(_wallConfig.from + v*t, position);
      }

      return result;
    }
   private:
    WallConfig _wallConfig;
  };
}