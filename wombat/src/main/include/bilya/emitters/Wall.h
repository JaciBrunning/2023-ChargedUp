#pragma once

#include "bilya/emitters/BaseEmitter.h"

namespace bilya {
  enum class WallSide {
    BOTH, POSITIVE, NEGATIVE
  };
  
  struct WallConfig {
    Position2d from, to;
    bool overflow = true;
    WallSide side = WallSide::BOTH;
  };

  class Wall : public Emitter {
   public:
    Wall(WallConfig wallConfig, EmitterConfig emitterConfig);

    PointGradient Calculate(const Position2d &position) const override;
   private:
    WallConfig _wallConfig;
  };
}